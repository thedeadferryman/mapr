//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include "view/types.hpp"

namespace kodgen::transform {

class TypeNameTransformer {
  public:
	enum class SlugType {
		NoSlug,
		CountSlug,
		FullSlug
	};

	[[nodiscard]] static auto getTypeName(
		std::shared_ptr<view::TypeBase> type) -> std::string;

	[[nodiscard]] static auto getOverloadSlug(
		std::shared_ptr<view::TypeBase> type) -> std::string;

  private:
	[[nodiscard]] static auto getUnqualifiedTypeName(
		std::shared_ptr<view::TypeBase> type) -> std::string;

	[[nodiscard]] static auto getUnqualifiedOverloadSlug(
		std::shared_ptr<view::TypeBase> type) -> std::string;

	[[nodiscard]] static auto getBuiltinOverloadSlug(
		const std::shared_ptr<view::BuiltinType>& builtinType) -> std::string;

	[[nodiscard]] static auto referenceKindSuffix(
		kodgen::view::ReferenceKind kind) -> std::string_view;

	[[nodiscard]] static auto getBuiltinTypeName(
		const std::shared_ptr<view::BuiltinType>& builtinType) -> std::string;

	[[nodiscard]] static auto getPointerTypeName(
		const std::shared_ptr<view::PointerType>& pointerType) -> std::string;

	[[nodiscard]] static auto getDeclarationReferenceTypeName(
		const std::shared_ptr<view::ReferenceType>& referenceType)
		-> std::string;

	[[nodiscard]] static auto getEnumTypeName(
		const std::shared_ptr<view::EnumType>& enumType) -> std::string;
};

}  // namespace kodgen::getName
