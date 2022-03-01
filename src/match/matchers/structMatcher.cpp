//
// Created by kmeinkopf on 15.12.2021.
//

#include "structMatcher.hpp"

using kodgen::match::StructMatcher;

using clang::ast_matchers::isStruct;
using clang::ast_matchers::recordDecl;

using clang::RecordDecl;

StructMatcher::StructMatcher(std::shared_ptr<view::DeclContext> context)
	: MatcherBase(std::move(context)) {}

void StructMatcher::run(
	const clang::ast_matchers::MatchFinder::MatchResult& result) {
	auto node = result.Nodes.getNodeAs<RecordDecl>("struc");

	llvm::errs() << *node << "\n";
}

void StructMatcher::bind(
	clang::ast_matchers::MatchFinder* matchFinder) {
	const auto matcher = recordDecl(isStruct()).bind("struc");

	matchFinder->addMatcher(matcher, this);
}
