//
// Created by kmeinkopf on 15.12.2021.
//

#include "match/matchers/matcherBase.hpp"

#include "view/function/functionDecl.hpp"
#include "view/record/recordDecl.hpp"

#pragma once
namespace mapr::match {

class RecordMatcher : public MatcherBase {
  public:
	explicit RecordMatcher(std::shared_ptr<view::DeclContext> context);

	void run(
		const clang::ast_matchers::MatchFinder::MatchResult& result) override;

	void bind(clang::ast_matchers::MatchFinder* matchFinder) override;
};

}  // namespace mapr::match
