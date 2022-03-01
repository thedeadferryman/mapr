//
// Created by kmeinkopf on 01.03.22.
//

#pragma once

#include "match/matcherBase.hpp"

namespace kodgen::match {

class ClassMatcher : public MatcherBase {
  public:
	explicit ClassMatcher(std::shared_ptr<view::DeclContext> ctx);

	void bind(clang::ast_matchers::MatchFinder* matchFinder) override;
	void run(
		const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
};

}  // namespace kodgen::match
