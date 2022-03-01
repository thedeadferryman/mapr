//
// Created by kmeinkopf on 01.03.22.
//

#include "classMatcher.hpp"

#include <clang/AST/AST.h>
#include <clang/ASTMatchers/ASTMatchers.h>

using clang::CXXRecordDecl;
using clang::ast_matchers::recordDecl;

using kodgen::match::ClassMatcher;

ClassMatcher::ClassMatcher(std::shared_ptr<view::DeclContext> ctx)
	: MatcherBase(std::move(ctx)) {}

void ClassMatcher::bind(clang::ast_matchers::MatchFinder* matchFinder) {
	matchFinder->addMatcher(recordDecl().bind("class"), this);
}

void ClassMatcher::run(
	const clang::ast_matchers::MatchFinder::MatchResult& result) {
	if (auto decl = result.Nodes.getNodeAs<CXXRecordDecl>("class")) {
		for (auto meth : decl->methods()) {}
	}
}
