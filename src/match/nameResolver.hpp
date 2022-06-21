//
// Created by kmeinkopf on 26.12.2021.
//

#pragma once

#include <clang/AST/DeclBase.h>

#include "view/name/qualifiedName.hpp"

namespace mapr::match {

class NameResolver {
  public:
	NameResolver() = delete;

	[[nodiscard]] static auto resolveDeclName(const clang::DeclContext* decl,
	                                          const clang::ASTContext* context)
		-> view::QualifiedName;

	 [[nodiscard]] static auto resolveNamedDeclName(
		const clang::NamedDecl* decl, const clang::ASTContext* context)
		-> view::QualifiedName;
};

}  // namespace mapr::match