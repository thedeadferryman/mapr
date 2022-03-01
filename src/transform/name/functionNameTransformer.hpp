//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include "view/function/functionDecl.hpp"

namespace kodgen::transform {

class FunctionNameTransformer {
  public:
	enum class SlugType {
		NoSlug,
		CountSlug,
		FullSlug
	};

	FunctionNameTransformer() = delete;

	[[nodiscard]] static auto getOverloadName(
		const view::NamespacedName& functionName) -> std::string;

	[[nodiscard]] static auto getOverloadName(
		const view::NamespacedName& functionName, std::size_t argCount)
		-> std::string;

	[[nodiscard]] static auto getOverloadName(
		const view::NamespacedName& functionName,
		const view::FunctionOverload& overload) -> std::string;

	[[nodiscard]] static auto getOverloadName(
		const view::NamespacedName& functionName,
		const view::FunctionOverload& overload,
		SlugType slugType) -> std::string;

  private:
	[[nodiscard]] static auto getOverloadTypedSuffix(const view::FunctionOverload& overload)
		-> std::string;

	[[nodiscard]] static auto getOverloadCountSuffix(std::size_t argCount) -> std::string;
};

}  // namespace kodgen::transform
