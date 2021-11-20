//
// Created by kmeinkopf on 20.11.2021.
//

#include "primitiveType.hpp"
kodgen::view::PrimitiveType::PrimitiveType(const std::string_view& name,
                                           const clang::QualType& type)
	: TypeBase(name, type) {}

kodgen::view::PrimitiveType::PrimitiveType(const clang::QualType& type)
	: PrimitiveType(type.getAsString(), type) {}
