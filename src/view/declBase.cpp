//
// Created by kmeinkopf on 20.11.2021.
//

#include "declBase.hpp"

using kodgen::view::DeclBase;

DeclBase::DeclBase(std::string_view id, DeclType declType)
	: id(id)
	, declType(declType) {};

auto DeclBase::getID() const -> const std::string& {
	return id;
}

auto DeclBase::getDeclType() const -> DeclType {
	return declType;
}