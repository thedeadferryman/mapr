//
// Created by kmeinkopf on 07.01.2022.
//

#pragma once

#include "match/matcherBase.hpp"

namespace kodgen::match {

class EnumMatcher : public MatcherBase {
  public:
	explicit EnumMatcher(std::shared_ptr<view::DeclContext> context);

	void run(
		const clang::ast_matchers::MatchFinder::MatchResult& result) override;

	void bind(
		clang::ast_matchers::MatchFinder* matchFinder) override;
};

}  // namespace kodgen::match
