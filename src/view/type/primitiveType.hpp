//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include "typeBase.hpp"

namespace kodgen::view {

class PrimitiveType : public TypeBase {
  public:
	PrimitiveType(const clang::QualType& type);

	PrimitiveType(const std::string_view& name, const clang::QualType& type);
};

}  // namespace kodgen::view