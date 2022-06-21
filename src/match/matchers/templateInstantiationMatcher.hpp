//
// Created by karl on 14.05.22.
//

#pragma once

#include <clang/Frontend/CompilerInstance.h>

#include "match/matchers/matcherBase.hpp"

namespace mapr::match {

class TemplateInstantiationMatcher : public MatcherBase {
	std::shared_ptr<clang::CompilerInstance> compiler;

  public:
	TemplateInstantiationMatcher(
		std::shared_ptr<view::DeclContext> context,
		std::shared_ptr<clang::CompilerInstance> compiler);

	void run(
		const clang::ast_matchers::MatchFinder::MatchResult& result) override;

	void bind(clang::ast_matchers::MatchFinder* matchFinder) override;

  private:
	[[nodiscard]] auto instantiateTemplate(
		const clang::ClassTemplateSpecializationDecl* templateSpec)
		-> const clang::CXXRecordDecl*;
};

}  // namespace mapr::match
