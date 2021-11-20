//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <utility>

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include "matchContext.hpp"

namespace kodgen::match {

template<typename T>
class MatcherBase : public clang::ast_matchers::MatchFinder::MatchCallback {
  public:
	explicit MatcherBase(std::shared_ptr<MatchContext> context)
		: context(std::move(context)) {};

	[[nodiscard]] virtual clang::ast_matchers::internal::Matcher<
		clang::Decl>
	getMatcher() const = 0;

  protected:
	std::shared_ptr<MatchContext> context;
};

}  // namespace kodgen::match
