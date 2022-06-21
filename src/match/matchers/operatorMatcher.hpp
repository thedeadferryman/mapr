//
// Created by karl on 13.04.22.
//

#pragma once

#include "match/matchers/matcherBase.hpp"

namespace mapr::match {

class OperatorMatcher : public MatcherBase {
  public:
	explicit OperatorMatcher(const std::shared_ptr<view::DeclContext> &context);

	void bind(clang::ast_matchers::MatchFinder* matchFinder) override;
	void run(
		const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
};

}  // namespace mapr::match
