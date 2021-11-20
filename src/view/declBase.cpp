//
// Created by kmeinkopf on 20.11.2021.
//

#include "declBase.hpp"

using kodgen::view::DeclBase;

DeclBase::DeclBase(const std::string_view& name)
	: name(name) {};

std::string_view DeclBase::getName() const {
	return std::string_view(name);
}

DeclBase::~DeclBase() = default;
