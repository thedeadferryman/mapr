//
// Created by kmeinkopf on 07.01.2022.
//

#include <utility>

#include "enumMatcher.hpp"

#include "match/typeResolver.hpp"

#include "view/enum/enumDecl.hpp"

using clang::ast_matchers::enumDecl;
using clang::ast_matchers::isExpansionInFileMatching;

using clang::ast_matchers::MatchFinder;

using kodgen::match::EnumMatcher;

using kodgen::view::EnumDecl;
using kodgen::view::EnumKind;

EnumMatcher::EnumMatcher(std::shared_ptr<view::DeclContext> context)
	: MatcherBase(std::move(context)) {}

void EnumMatcher::registerMatcher(MatchFinder* matchFinder) {
	const auto matcher =
		enumDecl(isExpansionInFileMatching("sample.cpp")).bind("enum");

	matchFinder->addMatcher(matcher, this);
}

void EnumMatcher::run(
	const clang::ast_matchers::MatchFinder::MatchResult& result) {
	const auto* enumNode = result.Nodes.getNodeAs<clang::EnumDecl>("enum");

	if (enumNode == nullptr) {
		return;
	}

	const std::shared_ptr<view::TypeBase>& assocType = TypeResolver::resolve(
		result.Context->getEnumType(enumNode), result.Context);

	if (auto enumType = std::dynamic_pointer_cast<view::EnumType>(assocType)) {
		auto decl = std::make_shared<EnumDecl>(enumType);

		for (auto* memberDecl : enumNode->enumerators()) {
			llvm::errs() << memberDecl->getNameAsString() << "\n";
			const std::shared_ptr<EnumDecl::Member>& member =
				std::make_shared<EnumDecl::Member>(
					memberDecl->getNameAsString(), decl);
			decl->addMember(member);
		}

		context->addDeclaration(decl);
	} else {
		UNREACHABLE();
	}
}
