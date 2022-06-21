//
// Created by karl on 22.03.22.
//

#pragma once

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include "match/matchers/matcherBase.hpp"

#include "view/declContext.hpp"

#include "config/toolProperties.hpp"

namespace mapr::app {

class SourceParser {
	config::ToolProperties config;
	std::shared_ptr<view::DeclContext> declContext;

	std::shared_ptr<clang::CompilerInstance> compiler;

	clang::ast_matchers::MatchFinder matchFinder;
	std::vector<std::shared_ptr<match::MatcherBase>> matchers;

  public:
	SourceParser(config::ToolProperties properties,
	             const std::vector<const char*>& args);

	void initialize();

	auto parseAll() -> bool;

	auto getParsedContext() const -> std::shared_ptr<view::DeclContext>;
};

}  // namespace mapr::app
