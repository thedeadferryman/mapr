//
// Created by karl on 13.04.22.
//

#include "operatorMatcher.hpp"

#include <clang/ASTMatchers/ASTMatchers.h>

#include "match/matchers/internal/clangMatchers.hpp"
#include "match/nameResolver.hpp"
#include "match/overloadMismatchException.hpp"
#include "match/typeResolver.hpp"

#include "view/function/operatorDecl.hpp"
#include "view/varDecl.hpp"

#include "match/matchers/bannedOperators.hpp"

using mapr::match::OperatorMatcher;

using mapr::view::FunctionOverload;
using mapr::view::OperatorDecl;
using mapr::view::VarDecl;

using clang::ast_matchers::allOf;
using clang::ast_matchers::cxxMethodDecl;
using clang::ast_matchers::functionDecl;
using clang::ast_matchers::unless;

OperatorMatcher::OperatorMatcher(
	const std::shared_ptr<view::DeclContext>& context)
	: MatcherBase(context) {}

void OperatorMatcher::bind(clang::ast_matchers::MatchFinder* matchFinder) {
	auto matcher = functionDecl(allOf(isOperator(), unless(cxxMethodDecl())))
					   .bind("operator");

	matchFinder->addMatcher(matcher, this);
}
void OperatorMatcher::run(
	const clang::ast_matchers::MatchFinder::MatchResult& result) {
	const auto* opDecl =
		result.Nodes.getNodeAs<clang::FunctionDecl>("operator");

	if (opDecl == nullptr
	    || isBannedOperator(opDecl->getOverloadedOperator())) {
		return;
	}

	auto argList = std::vector<std::shared_ptr<const VarDecl>>();

	for (auto* parm : opDecl->parameters()) {
		auto parmLoc = view::SingleSourceLoc::fromClangLoc(
			clang::FullSourceLoc(parm->getBeginLoc(), *result.SourceManager)  //
		);

		argList.push_back(  //
			std::make_shared<VarDecl>(
				parm->getName().str(),
				TypeResolver::resolve(parm->getType(), result.Context),
				std::move(parmLoc)  //
				));
	}

	const auto& declId = opDecl->getQualifiedNameAsString();

	auto opLoc = view::SingleSourceLoc::fromClangLoc(
		clang::FullSourceLoc(opDecl->getBeginLoc(), *result.SourceManager)  //
	);

	auto decl = FunctionOverload(
		TypeResolver::resolve(opDecl->getReturnType(), result.Context),
		argList,
		std::move(opLoc)  //
	);

	if (auto found = context->findDeclaration(declId)) {
		if (auto funOverload =
		        std::dynamic_pointer_cast<const OperatorDecl>(found)) {
			context->replaceDeclaration(funOverload->addOverload(decl));
		} else {
			throw OverloadMismatchException(
				funOverload->getQualifiedName().str(),
				funOverload->getLocation(),
				found->getDeclType(),
				view::DeclType::OperatorOverload,
				found->getLocation()  //
			);
		}
	} else {
		auto newOverload = std::make_shared<OperatorDecl>(
			declId,
			NameResolver::resolveDeclName(opDecl, result.Context),
			decl,
			opDecl->getOverloadedOperator());

		context->addDeclaration(newOverload);
	}
}
