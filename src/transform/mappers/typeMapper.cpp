//
// Created by kmeinkopf on 26.01.2022.
//

#include "typeMapper.hpp"

#include "view/types.hpp"

#include "transform/dependencies/dependencyRequest.hpp"
#include "transform/writers/defineConditionWriter.hpp"
#include "transform/writers/includeWriter.hpp"

#include "util/matchType.hpp"

using kodgen::transform::TypeMapper;
using kodgen::util::MatchType;

TypeMapper::TypeMapper(const std::shared_ptr<TypeDecl>& decl)
	: type(decl->getType()) {}

auto TypeMapper::checkDependencies() const
	-> std::vector<std::shared_ptr<DependencyRequest>> {
	return std::vector<std::shared_ptr<DependencyRequest>>();
}

void TypeMapper::writeDeclaration(WriterStream& writer) {
	MatchType::switchType(
		type,  //
		[&writer](const std::shared_ptr<view::BuiltinType>& builtinType) {
			writeBuiltinType(writer, builtinType);
		},
		[=](const std::shared_ptr<view::TypeBase>&) {});
}
void TypeMapper::writeDefinition(WriterStream& /*unused*/) {}

void TypeMapper::writeBuiltinType(
	WriterStream& writer, const std::shared_ptr<view::BuiltinType>& type) {
	switch (type->getVariant()) {
		case view::BuiltinType::Variant::Bool:
			writer << DefineConditionWriter::ifCXX(
				std::make_unique<IncludeWriter>("stdbool.h"), nullptr, true);
		default:
			return;
	}
}
