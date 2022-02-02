//
// Created by kmeinkopf on 26.01.2022.
//

#include "typeDecl.hpp"

using kodgen::transform::TypeDecl;

TypeDecl::TypeDecl(std::shared_ptr<view::TypeBase> type)
	: view::DeclBase(type->getPrettyName(), view::DeclType::Type)
	, type(std::move(type)) {}

auto TypeDecl::getType() -> std::shared_ptr<view::TypeBase> {
	return type;
}
