//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <clang/ASTMatchers/ASTMatchers.h>

#include "match/matcherBase.hpp"

namespace kodgen::match {

class PrimitiveFunctionMatcher : public MatcherBase<clang::Decl> {
  public:
	PrimitiveFunctionMatcher(std::shared_ptr<MatchContext> context);

	[[nodiscard]] auto getMatcher() const
		-> clang::ast_matchers::internal::Matcher<clang::Decl> override;

	void run(
		const clang::ast_matchers::MatchFinder::MatchResult& result) override;
};

}  // namespace kodgen::match
