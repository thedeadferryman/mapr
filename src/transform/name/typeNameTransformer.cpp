#include <functional>
#include <utility>

#include "typeNameTransformer.hpp"

#include "transform/exceptions/unsupportedTypeException.hpp"
#include "transform/name/qualifiedNameTransformer.hpp"

#include "util/matchType.hpp"
#include "util/stringBuilder.hpp"

using mapr::transform::TypeNameTransformer;
using mapr::transform::UnsupportedTypeException;
using mapr::util::MatchType;

constexpr auto unsupportedType =
	[](const std::shared_ptr<mapr::view::TypeBase>& type) -> std::string {
	throw UnsupportedTypeException(type->getKind(), type->getPrettyName());
};

mapr::transform::TypeNameTransformer::TypeNameTransformer(
	std::shared_ptr<config::PipelineContext> context)
	: context(std::move(context))
	, nameTransformer(this->context) {}

auto TypeNameTransformer::getTypeName(
	std::shared_ptr<view::TypeBase> type) const -> std::string {
	return MatchType::matchType<std::string>(
		std::move(type),  //
		[this](const std::shared_ptr<view::BuiltinType>& builtin) {
			return getBuiltinTypeName(builtin);
		},
		[this](const std::shared_ptr<view::PointerType>& ptr) {
			return getPointerTypeName(ptr);
		},
		[this](const std::shared_ptr<view::ReferenceType>& ref) {
			return getReferenceTypeName(ref);
		},
		[this](const std::shared_ptr<view::AliasType>& alias) {
			return nameTransformer.getName(alias->getName());
		},
		[this](const std::shared_ptr<view::EnumType>& enumType) {
			return getEnumTypeName(enumType);
		},
		[this](const std::shared_ptr<view::RecordType>& recordType) {
			return getRecordTypeName(recordType);
		},
		unsupportedType);
}

auto TypeNameTransformer::getOverloadSlug(
	const std::shared_ptr<view::TypeBase>& type) const -> std::string {
	auto builder = std::stringstream();

	builder << getUnqualifiedOverloadSlug(type);

	if (type->getQualifiers().hasVolatile()) {
		builder << "V";
	}

	if (type->getQualifiers().hasConst()) {
		builder << "C";
	}

	return builder.str();
}

auto TypeNameTransformer::getUnqualifiedOverloadSlug(
	std::shared_ptr<view::TypeBase> type) const -> std::string {
	return MatchType::matchType<std::string>(
		std::move(type),  //
		[this](const std::shared_ptr<view::PointerType>& ptr) -> std::string {
			return getOverloadSlug(ptr->getPointee()) + "P";
		},
		[this](const std::shared_ptr<view::ReferenceType>& ref) -> std::string {
			return getOverloadSlug(ref->dereference())
				+ std::string(referenceKindSuffix(ref->getReferenceKind()));
		},
		[this](const std::shared_ptr<view::AliasType>& alias) -> std::string {
			return nameTransformer.getName(alias->getName(), NameStyle::Raw);
		},
		[this](const std::shared_ptr<view::BuiltinType>& builtin)
			-> std::string { return getBuiltinOverloadSlug(builtin); },
		[this](const std::shared_ptr<view::EnumType>& enumType) -> std::string {
			return nameTransformer.getName(enumType->getName(), NameStyle::Raw);
		},
		[this](const std::shared_ptr<view::RecordType>& recordType)
			-> std::string {
			return nameTransformer.getName(recordType->getName(),
		                                   NameStyle::Raw);
		},
		unsupportedType);
}

auto TypeNameTransformer::referenceKindSuffix(
	mapr::view::ReferenceKind kind) const -> std::string_view {
	switch (kind) {
		case mapr::view::ReferenceKind::LVALUE:
			return "LV";
		case mapr::view::ReferenceKind::RVALUE:
			return "RV";
		default:
			return "R";
	}
}

auto TypeNameTransformer::getBuiltinOverloadSlug(
	const std::shared_ptr<view::BuiltinType>& builtinType) const
	-> std::string {
	switch (builtinType->getVariant()) {
		case clang::BuiltinType::Void:
			return "v";
		case clang::BuiltinType::NullPtr:
			return "n";

		case clang::BuiltinType::Bool:
			return "b";

		case clang::BuiltinType::Char_U:
		case clang::BuiltinType::Char_S:
			return "cc";
		case clang::BuiltinType::SChar:
			return "sc";
		case clang::BuiltinType::UChar:
			return "uc";

		case clang::BuiltinType::WChar_U:
		case clang::BuiltinType::WChar_S:
			return "wc";

		case clang::BuiltinType::Char8:
			return "c8";
		case clang::BuiltinType::Char16:
			return "c16";
		case clang::BuiltinType::Char32:
			return "c32";

		case clang::BuiltinType::UShort:
			return "us";
		case clang::BuiltinType::UInt:
			return "ui";
		case clang::BuiltinType::ULong:
			return "ul";
		case clang::BuiltinType::ULongLong:
			return "ull";
		case clang::BuiltinType::UInt128:
			return "u128";

		case clang::BuiltinType::Short:
			return "si";
		case clang::BuiltinType::Int:
			return "ii";
		case clang::BuiltinType::Long:
			return "li";
		case clang::BuiltinType::LongLong:
			return "lli";
		case clang::BuiltinType::Int128:
			return "i128";

		case clang::BuiltinType::Float:
			return "lf";
		case clang::BuiltinType::Double:
			return "ld";
		case clang::BuiltinType::LongDouble:
			return "lld";
		case clang::BuiltinType::Float16:
			return "f16";
		case clang::BuiltinType::Float128:
			return "f128";

		default:
			throw UnsupportedTypeException(builtinType->getVariant(),
			                               builtinType->getDisplayName());
	}
}

auto TypeNameTransformer::getBuiltinTypeName(
	const std::shared_ptr<view::BuiltinType>& builtinType,
	QualifiersMode qualifiersMode) const -> std::string {
	auto builder = std::stringstream();
	if (qualifiersMode == QualifiersMode::Keep) {
		if (builtinType->getQualifiers().hasVolatile()) {
			builder << "volatile ";
		}

		if (builtinType->getQualifiers().hasConst()) {
			builder << "const ";
		}
	}

	builder
		<< builtinType->getDisplayName();  // TODO Review `displayName` behavior

	return builder.str();
}

auto TypeNameTransformer::getPointerTypeName(
	const std::shared_ptr<view::PointerType>& pointerType) const
	-> std::string {
	auto builder = std::stringstream();

	builder << getTypeName(pointerType->getPointee()) << " *";

	if (pointerType->getQualifiers().hasVolatile()) {
		builder << " volatile";
	}

	if (pointerType->getQualifiers().hasConst()) {
		builder << " const";
	}

	return builder.str();
}

/// We don't have references in C, so we're using pointers instead
auto TypeNameTransformer::getReferenceTypeName(
	const std::shared_ptr<view::ReferenceType>& referenceType) const
	-> std::string {
	auto target = referenceType->dereference();

	if (target->getKind() == view::TypeKind::Builtin
	    && target->getQualifiers().hasConst()) {
		if (auto builtinType =
		        std::dynamic_pointer_cast<view::BuiltinType>(target)) {
			return getBuiltinTypeName(builtinType, QualifiersMode::Remove);
		}

		UNREACHABLE_M("Type kind is Builtin, but class isn't!");
	}

	if (target->getKind() == view::TypeKind::Pointer) {
		return getTypeName(target);
	}

	return getTypeName(target) + " *";
}

auto TypeNameTransformer::getEnumTypeName(
	const std::shared_ptr<view::EnumType>& enumType) const -> std::string {
	auto builder = util::StringBuilder();

	builder << nameTransformer.getName(enumType->getName());

	if (enumType->getQualifiers().hasConst()) {
		builder << " const";
	}
	if (enumType->getQualifiers().hasVolatile()) {
		builder << " volatile";
	}

	return builder;
}

auto TypeNameTransformer::getRecordTypeName(
	const std::shared_ptr<view::RecordType>& recordType) const -> std::string {
	auto builder = util::StringBuilder();

	builder << nameTransformer.getName(recordType->getName())
			<< context->readConfigVariable("classTypeSuffix").value_or("_CT");

	if (recordType->getQualifiers().hasConst()) {
		builder << " const";
	}
	if (recordType->getQualifiers().hasVolatile()) {
		builder << " volatile";
	}

	return builder;
}

[[nodiscard]] auto mapAliasType(
	const std::shared_ptr<mapr::view::AliasType>& aliasType)
	-> std::shared_ptr<mapr::view::TypeBase>;

auto TypeNameTransformer::buildMappedType(
	std::shared_ptr<view::TypeBase> type) const -> std::string {
	auto mappedType = MatchType::matchType<std::shared_ptr<view::TypeBase>>(
		std::move(type),
		[](const std::shared_ptr<view::RecordType>& recordType)
			-> std::shared_ptr<view::TypeBase> {
			return recordType->asPointer();
		},
		[](const std::shared_ptr<view::AliasType>& aliasType)
			-> std::shared_ptr<view::TypeBase> {
			return mapAliasType(aliasType);
		},
		[](std::shared_ptr<view::TypeBase> typeBase) { return typeBase; });

	return getTypeName(mappedType);
}

[[nodiscard]] auto isAliasOfRecord(
	const std::shared_ptr<mapr::view::AliasType>& aliasType) -> bool;

auto mapAliasType(const std::shared_ptr<mapr::view::AliasType>& aliasType)
	-> std::shared_ptr<mapr::view::TypeBase> {
	auto target = aliasType->getSource();

	if (isAliasOfRecord(aliasType)) {
		return std::make_shared<mapr::view::PointerType>(aliasType);
	}

	return aliasType;
}

auto isAliasOfRecord(const std::shared_ptr<mapr::view::AliasType>& aliasType)
	-> bool {
	auto current = aliasType->getSource();

	while (true) {
		if (auto recordType =
		        std::dynamic_pointer_cast<mapr::view::RecordType>(current)) {
			return true;
		}
		if (auto inAliasType =
		        std::dynamic_pointer_cast<mapr::view::AliasType>(current)) {
			current = inAliasType->getSource();
			continue;
		}
		return false;
	}
}
