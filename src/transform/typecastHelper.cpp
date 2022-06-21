//
// Created by karl on 06.04.22.
//

#include "typecastHelper.hpp"

#include "view/types/builtinType.hpp"
#include "view/types/recordType.hpp"

#include "transform/name/typeNameTransformer.hpp"
#include "transform/writers/dereferenceWriter.hpp"
#include "transform/writers/dynCtorInvocationWriter.hpp"
#include "transform/writers/pointerizeWriter.hpp"
#include "transform/writers/pointerTypeWriter.hpp"
#include "transform/writers/sequentialWriter.hpp"
#include "transform/writers/textWriter.hpp"

#include "util/matchType.hpp"

using mapr::transform::CastStyle;
using mapr::transform::CXXCastWriter;
using mapr::transform::DereferenceWriter;
using mapr::transform::DynCtorInvocationWriter;
using mapr::transform::PointerTypeWriter;
using mapr::transform::SequentialWriter;
using mapr::transform::TextWriter;
using mapr::transform::TypecastHelper;
using mapr::transform::TypeNameTransformer;
using mapr::transform::WriterBase;

using mapr::view::BuiltinType;
using mapr::view::PointerType;
using mapr::view::RecordType;
using mapr::view::ReferenceType;
using mapr::view::TypeKind;

using mapr::util::MatchType;

[[nodiscard]] auto castRecordFromMapping(
	const std::shared_ptr<RecordType>& type, std::unique_ptr<WriterBase> value)
	-> std::unique_ptr<WriterBase>;

[[nodiscard]] auto castPointerFromMapping(
	const std::shared_ptr<PointerType>& ptrType,
	std::unique_ptr<WriterBase> value) -> std::unique_ptr<WriterBase>;

TypecastHelper::TypecastHelper(std::shared_ptr<config::PipelineContext> context)
	: context(std::move(context)) {}

auto TypecastHelper::castToMapping(const std::shared_ptr<view::TypeBase>& type,
                                   std::unique_ptr<WriterBase> value) const
	-> std::unique_ptr<WriterBase> {
	auto typeTransformer = TypeNameTransformer(context);
	return MatchType::matchType<std::unique_ptr<WriterBase>>(
		type,
		[&value,
	     typeTransformer](const std::shared_ptr<view::EnumType>& enumType)
			-> std::unique_ptr<WriterBase> {
			return std::make_unique<CXXCastWriter>(
				std::move(value),
				std::make_unique<TextWriter>(
					typeTransformer.buildMappedType(enumType)));
		},
		[&value,
	     typeTransformer](const std::shared_ptr<view::RecordType>& recordType)
			-> std::unique_ptr<WriterBase> {
			auto args = std::vector<std::unique_ptr<WriterBase>>();

			args.push_back(std::move(value));

			return std::make_unique<CXXCastWriter>(
				std::make_unique<DynCtorInvocationWriter>(  //
					std::make_unique<TextWriter>(recordType->getName().str()),
					std::move(args)  //
					),
				std::make_unique<TextWriter>(
					typeTransformer.buildMappedType(recordType)),
				CastStyle::ReinterpretCast);
		},
		[&value,
	     this](const std::shared_ptr<view::ReferenceType>& referenceType) {
			return castReferenceToMapping(referenceType, std::move(value));
		},
		[&value, this](const std::shared_ptr<view::PointerType>& ptrType) {
			return castPointerToMapping(ptrType, std::move(value));
		},
		[&value, this](const std::shared_ptr<view::AliasType>& aliasType) {
			return castToMapping(aliasType->getSource(), std::move(value));
		},
		[&value](const std::shared_ptr<view::TypeBase>& /*unused*/) {
			return std::move(value);
		});
}

auto TypecastHelper::castFromMapping(
	const std::shared_ptr<view::TypeBase>& type,
	std::unique_ptr<WriterBase> value) const -> std::unique_ptr<WriterBase> {
	return MatchType::matchType<std::unique_ptr<WriterBase>>(
		type,
		[&value](const std::shared_ptr<view::RecordType>& recordType)
			-> std::unique_ptr<WriterBase> {
			return castRecordFromMapping(recordType, std::move(value));
		},
		[&value](const std::shared_ptr<view::PointerType>& ptrType)
			-> std::unique_ptr<WriterBase> {
			return castPointerFromMapping(ptrType, std::move(value));
		},
		[&value, this](const std::shared_ptr<view::ReferenceType>& refType)
			-> std::unique_ptr<WriterBase> {
			return castReferenceFromMapping(refType, std::move(value));
		},
		[&value](const std::shared_ptr<view::EnumType>& enumType)
			-> std::unique_ptr<WriterBase> {
			return std::make_unique<CXXCastWriter>(
				std::move(value),  //
				std::make_unique<TextWriter>(enumType->getName().str())  //
			);
		},
		[&value, this](const std::shared_ptr<view::AliasType>& aliasType) {
			return castFromMapping(aliasType->getSource(), std::move(value));
		},
		[&value](const std::shared_ptr<view::TypeBase>& /* unused */)
			-> std::unique_ptr<WriterBase> { return std::move(value); });
}

auto TypecastHelper::castReferenceFromMapping(
	const std::shared_ptr<ReferenceType>& refType,
	std::unique_ptr<WriterBase> value) const -> std::unique_ptr<WriterBase> {
	auto targetType = refType->dereference();

	if (targetType->getKind() == view::TypeKind::Builtin
	    && targetType->getQualifiers().hasConst()) {
		return value;
	}

	if (auto ptrType = std::dynamic_pointer_cast<PointerType>(targetType)) {
		return castPointerFromMapping(ptrType, std::move(value));
	}

	auto innerWriter = castFromMapping(targetType, std::move(value));

	if (typeid(*innerWriter.get()) == typeid(DereferenceWriter)) {
		return innerWriter;
	}

	return std::make_unique<DereferenceWriter>(std::move(innerWriter));
}
auto TypecastHelper::castReferenceToMapping(
	const std::shared_ptr<view::ReferenceType>& refType,
	std::unique_ptr<WriterBase> value) const -> std::unique_ptr<WriterBase> {
	auto target = refType->dereference();

	if (target->getKind() == view::TypeKind::Builtin
	    && target->getQualifiers().hasConst()) {
		return value;
	}

	if (auto recordType = std::dynamic_pointer_cast<RecordType>(target)) {
		return castToMapping(recordType, std::move(value));
	}

	if (auto ptrType = std::dynamic_pointer_cast<PointerType>(target)) {
		return castToMapping(ptrType, std::move(value));
	}

	auto seq = std::vector<std::unique_ptr<WriterBase>>();

	seq.push_back(std::make_unique<TextWriter>("/* "));
	seq.push_back(std::make_unique<TextWriter>(refType->getPrettyName()));
	seq.push_back(std::make_unique<TextWriter>(" */"));
	seq.push_back(std::move(value));

	return std::make_unique<PointerizeWriter>(
		std::make_unique<SequentialWriter>(std::move(seq)));
}

auto TypecastHelper::castPointerToMapping(
	const std::shared_ptr<view::PointerType>& ptrType,
	std::unique_ptr<WriterBase> value) const -> std::unique_ptr<WriterBase> {
	if (auto builtinType =
	        std::dynamic_pointer_cast<BuiltinType>(ptrType->getPointee())) {
		return value;
	}

	auto typenameXformer = TypeNameTransformer(context);

	auto pointee = ptrType->getPointee();

	auto typeToCastSequence = std::vector<std::unique_ptr<WriterBase>>();

	typeToCastSequence.push_back(
		std::make_unique<TextWriter>(typenameXformer.getTypeName(pointee)));

	typeToCastSequence.push_back(std::make_unique<TextWriter>("*"));

	return std::make_unique<CXXCastWriter>(
		std::move(value),
		std::make_unique<SequentialWriter>(std::move(typeToCastSequence)  //
	                                       ),
		CastStyle::ReinterpretCast  //
	);
}

auto castPointerFromMapping(
	const std::shared_ptr<mapr::view::PointerType>& ptrType,
	std::unique_ptr<WriterBase> value) -> std::unique_ptr<WriterBase> {
	if (auto builtinType =
	        std::dynamic_pointer_cast<BuiltinType>(ptrType->getPointee())) {
		return value;
	}

	auto pointee = ptrType->getPointee();

	auto typeToCastSequence = std::vector<std::unique_ptr<WriterBase>>();

	typeToCastSequence.push_back(
		std::make_unique<TextWriter>(pointee->getPrettyName()));

	if (pointee->getQualifiers().hasConst()) {
		typeToCastSequence.push_back(std::make_unique<TextWriter>(" const"));
	}

	if (pointee->getQualifiers().hasVolatile()) {
		typeToCastSequence.push_back(std::make_unique<TextWriter>(" volatile"));
	}

	typeToCastSequence.push_back(std::make_unique<TextWriter>("*"));

	return std::make_unique<CXXCastWriter>(
		std::move(value),
		std::make_unique<SequentialWriter>(std::move(typeToCastSequence)  //
	                                       ),
		CastStyle::ReinterpretCast  //
	);
}

auto castRecordFromMapping(const std::shared_ptr<RecordType>& type,
                           std::unique_ptr<WriterBase> value)
	-> std::unique_ptr<WriterBase> {
	auto typeTo = std::make_unique<PointerTypeWriter>(
		type->getQualifiers(),
		std::make_unique<TextWriter>(type->getName().str()));

	auto castExpr = std::make_unique<CXXCastWriter>(
		std::move(value), std::move(typeTo), CastStyle::ReinterpretCast);

	return std::make_unique<DereferenceWriter>(std::move(castExpr));
}
