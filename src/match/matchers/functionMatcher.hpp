//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <clang/ASTMatchers/ASTMatchers.h>

#include "match/matchers/matcherBase.hpp"

namespace mapr::match {

class FunctionMatcher : public MatcherBase {
  public:
	explicit FunctionMatcher(std::shared_ptr<view::DeclContext> context);

	void run(
		const clang::ast_matchers::MatchFinder::MatchResult& result) override;

	void bind(
		clang::ast_matchers::MatchFinder* matchFinder) override;
};

}  // namespace mapr::match
