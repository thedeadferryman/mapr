//
// Created by kmeinkopf on 20.11.2021.
//

#include <utility>

#include "functionMatcher.hpp"

#include "match/nameResolver.hpp"
#include "match/matchers/internal/clangMatchers.hpp"
#include "match/typeResolver.hpp"

#include "view/function/functionDecl.hpp"
#include "view/function/functionOverload.hpp"

using DeclKind = clang::Decl::Kind;

using clang::ast_matchers::allOf;
using clang::ast_matchers::anyOf;
using clang::ast_matchers::autoType;
using clang::ast_matchers::builtinType;
using clang::ast_matchers::cxxMethodDecl;
using clang::ast_matchers::functionDecl;
using clang::ast_matchers::functionTemplateDecl;
using clang::ast_matchers::hasDeducedType;
using clang::ast_matchers::hasParent;
using clang::ast_matchers::isExpansionInFileMatching;
using clang::ast_matchers::MatchFinder;
using clang::ast_matchers::unless;

using kodgen::match::FunctionMatcher;
using kodgen::match::TypeResolver;

using kodgen::view::FunctionDecl;
using kodgen::view::FunctionOverload;
using kodgen::view::VarDecl;

FunctionMatcher::FunctionMatcher(std::shared_ptr<view::DeclContext> context)
	: MatcherBase(std::move(context)) {}

void FunctionMatcher::run(
	const clang::ast_matchers::MatchFinder::MatchResult& result  //
) {
	const auto* node =  //
		result.Nodes.getNodeAs<clang::FunctionDecl>("func");

	if (node == nullptr) {
		return;
	}

	auto argList = std::vector<std::shared_ptr<VarDecl>>();

	for (auto* parm : node->parameters()) {
		argList.push_back(std::make_shared<VarDecl>(
			parm->getName().str(),
			TypeResolver::resolve(parm->getType(), result.Context)));
	}

	const auto& declId = node->getQualifiedNameAsString();

	auto decl = FunctionOverload(
		TypeResolver::resolve(node->getReturnType(), result.Context),
		argList  //
	);

	if (auto found = context->findDeclaration(declId);
	    auto funOverload = std::dynamic_pointer_cast<FunctionDecl>(found)) {
		funOverload->addOverload(decl);
	} else {
		auto newOverload = std::make_shared<FunctionDecl>(
			declId, NameResolver::resolveDeclName(node), decl);

		context->addDeclaration(newOverload);
	}
}
void kodgen::match::FunctionMatcher::bind(
	clang::ast_matchers::MatchFinder* matchFinder) {
	const clang::ast_matchers::DeclarationMatcher matcher =
		allOf(functionDecl(  //
				  )
	              .bind("func"),
	          // discard member functions, since
	          // those are processed separately
	          unless(cxxMethodDecl()),
	          // discard function templates, since
	          // those are processed separately
	          unless(hasParent(functionTemplateDecl())));

	matchFinder->addMatcher(matcher, this);
}
