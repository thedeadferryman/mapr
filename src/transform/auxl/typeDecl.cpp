//
// Created by kmeinkopf on 26.01.2022.
//

#include "typeDecl.hpp"

#include "view/loc/sparseSourceLoc.hpp"
#include "view/name/qualifiedName.hpp"

using mapr::transform::TypeDecl;

TypeDecl::TypeDecl(std::shared_ptr<view::TypeBase> type)
	: view::DeclBase(type->getPrettyName(), view::DeclType::Type)
	, type(std::move(type)) {}

auto TypeDecl::getType() const -> std::shared_ptr<view::TypeBase> {
	return type;
}

auto TypeDecl::getLocation() const -> std::shared_ptr<view::SourceLoc> {
	return std::make_shared<view::SparseSourceLoc>();
}

auto TypeDecl::getQualifiedName() const -> view::QualifiedName {
	return view::QualifiedName("<type>") / getID();
}
