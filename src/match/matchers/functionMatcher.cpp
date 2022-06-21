//
// Created by kmeinkopf on 20.11.2021.
//

#include <utility>

#include "functionMatcher.hpp"

#include "match/matchers/internal/clangMatchers.hpp"
#include "match/matchers/internal/functionHelpers.hpp"
#include "match/nameResolver.hpp"
#include "match/overloadMismatchException.hpp"
#include "match/typeResolver.hpp"

#include "view/function/functionDecl.hpp"
#include "view/function/functionOverload.hpp"

using clang::ast_matchers::allOf;
using clang::ast_matchers::cxxMethodDecl;
using clang::ast_matchers::functionDecl;
using clang::ast_matchers::unless;

using mapr::match::FunctionMatcher;

using mapr::view::FunctionDecl;
using mapr::view::FunctionOverload;
using mapr::view::VarDecl;

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

	auto argList = std::vector<std::shared_ptr<const VarDecl>>();

	for (auto* parm : node->parameters()) {
		auto parmLoc = view::SingleSourceLoc::fromClangLoc(
			clang::FullSourceLoc(parm->getBeginLoc(), *result.SourceManager)  //
		);

		argList.push_back(  //
			std::make_shared<VarDecl>(
				FunctionHelpers::getParmName(parm),
				TypeResolver::resolve(parm->getType(), result.Context),
				std::move(parmLoc)  //
				));
	}

	auto qualName = NameResolver::resolveDeclName(node, result.Context);

	if (!qualName.isValid()) {
		llvm::errs() << qualName.str() << "\n";
	}

	auto fnLoc = view::SingleSourceLoc::fromClangLoc(
		clang::FullSourceLoc(node->getBeginLoc(), *result.SourceManager)  //
	);

	auto decl = FunctionOverload(
		TypeResolver::resolve(node->getReturnType(), result.Context),
		argList,
		std::move(fnLoc)  //
	);

	if (auto found = context->findDeclaration(qualName.str())) {
		if (auto funOverload =
		        std::dynamic_pointer_cast<const FunctionDecl>(found)) {
			context->replaceDeclaration(funOverload->addOverload(decl));
		} else {
			throw OverloadMismatchException(
				funOverload->getQualifiedName().str(),
				funOverload->getLocation(),
				found->getDeclType(),
				view::DeclType::Function,
				found->getLocation()  //
			);
		}
	} else {
		auto newOverload =
			std::make_shared<FunctionDecl>(qualName.str(), qualName, decl);

		context->addDeclaration(newOverload);
	}
}

void mapr::match::FunctionMatcher::bind(
	clang::ast_matchers::MatchFinder* matchFinder) {
	const clang::ast_matchers::DeclarationMatcher matcher =
		allOf(functionDecl(
				  // discard overloaded operators, since
	              // those are processed separately
				  unless(isOperator()))
	              .bind("func"),
	          // discard member functions, since
	          // those are processed separately
	          unless(cxxMethodDecl())  //
	    );

	matchFinder->addMatcher(matcher, this);
}
