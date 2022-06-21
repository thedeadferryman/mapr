//
// Created by kmeinkopf on 07.01.2022.
//

#include "typedefMatcher.hpp"

#include "view/types/aliasType.hpp"
#include "view/types/typeBase.hpp"

using clang::ast_matchers::isExpansionInFileMatching;
using clang::ast_matchers::typedefNameDecl;
using clang::ast_matchers::internal::Matcher;

using mapr::view::AliasType;
using mapr::view::TypeBase;

using mapr::match::TypedefMatcher;

TypedefMatcher::TypedefMatcher(std::shared_ptr<view::DeclContext> context)
	: MatcherBase(std::move(context)) {}

void TypedefMatcher::bind(
	clang::ast_matchers::MatchFinder* matchFinder) {
	const auto matcher =
		typedefNameDecl(isExpansionInFileMatching("sample.hpp"));

	matchFinder->addMatcher(matcher, this);
}

void TypedefMatcher::run(
	const clang::ast_matchers::MatchFinder::MatchResult& result) {}
