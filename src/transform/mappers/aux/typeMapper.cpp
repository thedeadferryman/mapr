//
// Created by kmeinkopf on 26.01.2022.
//

#include "typeMapper.hpp"

#include <llvm/Support/raw_ostream.h>

#include "view/types.hpp"
#include "view/types/recordType.hpp"

#include "transform/dependencies/declRequest.hpp"
#include "transform/dependencies/dependencyRequest.hpp"
#include "transform/dependencies/typeRequest.hpp"
#include "transform/name/typeNameTransformer.hpp"
#include "transform/writers/defineConditionWriter.hpp"
#include "transform/writers/includeWriter.hpp"
#include "transform/writers/statement/plainStatementWriter.hpp"
#include "transform/writers/textWriter.hpp"
#include "transform/writers/typedefWriter.hpp"

#include "util/matchType.hpp"

using mapr::transform::TypeMapper;
using mapr::util::MatchType;

TypeMapper::TypeMapper(const std::shared_ptr<const TypeDecl>& decl,
                       std::shared_ptr<config::PipelineContext> context,
                       MapperFactoryMode mode)
	: type(decl->getType())
	, context(std::move(context))
	, mode(mode) {}

auto TypeMapper::checkDependencies() const
	-> std::vector<std::shared_ptr<DependencyRequest>> {
	if (auto solvedType = inferRequiredType(type)) {
		return {solvedType};
	}

	return {};
}

void TypeMapper::write(WriterStream& writer) {
	if (mode != MapperFactoryMode::Declaration) {
		writer << TextWriter("/* type ")  //
			   << TextWriter(type->getPrettyName())  //
			   << TextWriter(" */\n");
	}

	MatchType::switchType(
		type,  //
		[&writer](const std::shared_ptr<view::BuiltinType>& builtinType) {
			writeBuiltinType(writer, builtinType);
		},
		[&writer, this](const std::shared_ptr<view::AliasType>& aliasType) {
			auto typeNameTransformer = TypeNameTransformer(context);

			writer << TypedefWriter(
				std::make_unique<TextWriter>(
					typeNameTransformer.getTypeName(aliasType->getSource())),
				std::make_unique<TextWriter>(
					typeNameTransformer.getTypeName(aliasType)));
		},
		[](const std::shared_ptr<view::TypeBase>&) {});
}

void TypeMapper::writeBuiltinType(
	WriterStream& writer, const std::shared_ptr<view::BuiltinType>& type) {
	switch (type->getVariant()) {
		case view::BuiltinType::Variant::Bool:
			writer << DefineConditionWriter::ifCXX(
				std::make_unique<IncludeWriter>("stdbool.h"),
				nullptr,
				ConditionType::IfUndefined);
		default:
			return;
	}
}
auto TypeMapper::inferRequiredType(const std::shared_ptr<view::TypeBase>& type)
	-> std::shared_ptr<DependencyRequest> {
	return MatchType::matchType<std::shared_ptr<DependencyRequest>>(
		type,  //
		[](const std::shared_ptr<view::EnumType>& enumType)
			-> std::shared_ptr<DependencyRequest> {
			return std::make_shared<DeclRequest>(enumType->getPrettyName());
		},
		[](const std::shared_ptr<view::RecordType>& recordType)
			-> std::shared_ptr<DependencyRequest> {
			return std::make_shared<DeclRequest>(recordType->getPrettyName());
		},
		[](const std::shared_ptr<view::PointerType>& ptrType) {
			return std::make_shared<TypeRequest>(ptrType->getPointee());
		},
		[](const std::shared_ptr<view::ReferenceType>& refType) {
			return std::make_shared<TypeRequest>(refType->dereference());
		},
		[](const std::shared_ptr<view::AliasType>& aliasType) {
			return std::make_shared<TypeRequest>(aliasType->getSource());
		},
		[](const std::shared_ptr<view::TypeBase>&) {
			return std::shared_ptr<DependencyRequest>(nullptr);
		});
}
