//
// Created by kmeinkopf on 07.01.2022.
//

#include "typedefMatcher.hpp"

#include "view/types/aliasType.hpp"
#include "view/types/typeBase.hpp"

using clang::ast_matchers::isExpansionInFileMatching;
using clang::ast_matchers::typedefNameDecl;
using clang::ast_matchers::internal::Matcher;

using kodgen::view::AliasType;
using kodgen::view::TypeBase;

using kodgen::match::TypedefMatcher;

TypedefMatcher::TypedefMatcher(std::shared_ptr<view::DeclContext> context)
	: MatcherBase(std::move(context)) {}

void TypedefMatcher::registerMatcher(
	clang::ast_matchers::MatchFinder* matchFinder) {
	const auto matcher =
		typedefNameDecl(isExpansionInFileMatching("sample.cpp"));

	matchFinder->addMatcher(matcher, this);
}

void TypedefMatcher::run(
	const clang::ast_matchers::MatchFinder::MatchResult& result) {}
