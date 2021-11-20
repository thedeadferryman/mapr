//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <clang/AST/Type.h>

#include "view/declBase.hpp"

namespace kodgen::view {

class TypeBase : public DeclBase {
	const clang::QualType type;

  public:
	TypeBase(const std::string_view& name, const clang::QualType& type);

	[[nodiscard]] clang::QualType getType() const;
};

}  // namespace kodgen::view