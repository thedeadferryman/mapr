//
// Created by kmeinkopf on 15.01.2022.
//

#pragma once

#include <optional>

#include <clang/AST/ASTContext.h>
#include <clang/AST/Type.h>

#include "view/name/qualifiedName.hpp"
#include "view/types/typeBase.hpp"

namespace mapr::match {

class TypeResolver {
  public:
	[[nodiscard]] static auto resolve(const clang::QualType& type,
	                                  const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;

  private:
	[[nodiscard]] static auto resolveBuiltin(
		const clang::QualType& type, const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;
	[[nodiscard]] static auto resolveRecord(
		const clang::QualType& type,
		const clang::ASTContext* context,
		std::optional<view::QualifiedName> overrideName = std::nullopt)
		-> std::shared_ptr<view::TypeBase>;
	[[nodiscard]] static auto resolvePointer(
		const clang::QualType& type, const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;
	[[nodiscard]] static auto resolveReference(
		const clang::QualType& type, const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;
	[[nodiscard]] static auto resolveAlias(const clang::QualType& type,
	                                       const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;
	[[nodiscard]] static auto resolveEnum(const clang::QualType& type,
	                                      const clang::ASTContext* astContext)
		-> std::shared_ptr<view::TypeBase>;
};

}  // namespace mapr::match
