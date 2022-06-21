//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <utility>

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include "view/declContext.hpp"

namespace mapr::match {

class MatcherBase : public clang::ast_matchers::MatchFinder::MatchCallback {
  public:
	explicit MatcherBase(std::shared_ptr<view::DeclContext> context)
		: context(std::move(context)) {};

	virtual void bind(
		clang::ast_matchers::MatchFinder* matchFinder) = 0;

  protected:
	std::shared_ptr<view::DeclContext> context;
};

}  // namespace mapr::match
