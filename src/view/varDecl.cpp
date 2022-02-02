//
// Created by kmeinkopf on 06.01.2022.
//

#include "varDecl.hpp"

using kodgen::view::VarDecl;

VarDecl::VarDecl(std::string_view id, std::shared_ptr<TypeBase> type)
	: DeclBase(id, DeclType::Enum)
	, type(std::move(type)) {}

auto VarDecl::getType() const -> const std::shared_ptr<TypeBase>& {
	return type;
}
