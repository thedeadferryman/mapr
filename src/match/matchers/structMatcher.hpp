//
// Created by kmeinkopf on 15.12.2021.
//

#include "match/matcherBase.hpp"

#pragma once
namespace kodgen::match {

class StructMatcher : public MatcherBase {
  public:
	explicit StructMatcher(std::shared_ptr<view::DeclContext> context);

	void run(
		const clang::ast_matchers::MatchFinder::MatchResult& result) override;

	void registerMatcher(
		clang::ast_matchers::MatchFinder* matchFinder) override;
};

}  // namespace kodgen::match
