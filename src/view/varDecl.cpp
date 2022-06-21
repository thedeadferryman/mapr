//
// Created by kmeinkopf on 06.01.2022.
//

#include <utility>

#include "varDecl.hpp"

#include "view/name/qualifiedName.hpp"

using mapr::view::VarDecl;

VarDecl::VarDecl(std::string_view id,
                 std::shared_ptr<TypeBase> type,
                 std::shared_ptr<SourceLoc> location)
	: DeclBase(id, DeclType::Enum)
	, type(std::move(type))
	, location(std::move(location)) {}

auto VarDecl::getType() const -> const std::shared_ptr<TypeBase>& {
	return type;
}

auto VarDecl::getLocation() const -> std::shared_ptr<SourceLoc> {
	return location;
}

auto VarDecl::getQualifiedName() const -> QualifiedName {
	return QualifiedName(getID());
}
