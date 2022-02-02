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

	[[nodiscard]] static auto getOverloadName(const view::NamespacedName& functionName,
	                            const view::FunctionOverload& overload,
	                            SlugType slugType) -> std::string;
  private:
	static auto functionParamSuffix(const view::FunctionOverload& overload,
	                                SlugType slugType) -> std::string;
};

}  // namespace kodgen::getName
