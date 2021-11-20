//
// Created by kmeinkopf on 20.11.2021.
//

#include "typeBase.hpp"

using kodgen::view::TypeBase;

TypeBase::TypeBase(const std::string_view& name, const clang::QualType& type)
	: DeclBase(name)
	, type(type) {}

clang::QualType TypeBase::getType() const {
	return type;
}
