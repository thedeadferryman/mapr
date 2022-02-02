//
// Created by kmeinkopf on 26.12.2021.
//

#pragma once

#include <clang/AST/DeclBase.h>

#include "view/namespacedName.hpp"

namespace kodgen::match {

class NameResolver {
  public:
	NameResolver() = delete;

	static auto resolveDeclName(const clang::DeclContext* decl)
		-> kodgen::view::NamespacedName;
};

}  // namespace kodgen::match