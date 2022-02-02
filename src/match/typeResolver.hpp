//
// Created by kmeinkopf on 15.01.2022.
//

#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/Type.h>

#include "view/types/typeBase.hpp"

namespace kodgen::match {

class TypeResolver {
  public:
	[[nodiscard]] static auto resolve(const clang::QualType& type,
	                                  const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;

  private:
	[[nodiscard]] static auto resolveBuiltin(
		const clang::QualType& type, const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;
	[[nodiscard]] static auto resolvePointer(
		const clang::QualType& type, const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;
	[[nodiscard]] static auto resolveReference(
		const clang::QualType& type, const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;
	[[nodiscard]] static auto resolveTypedef(
		const clang::QualType& type, const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;
	[[nodiscard]] static auto resolveEnum(const clang::QualType& type,
	                                      const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;
};

}  // namespace kodgen::match
