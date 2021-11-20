//
// Created by kmeinkopf on 20.11.2021.
//

#include <utility>

#include "primitiveFunctionMatcher.hpp"

#include "match/matchers/internal/clangMatchers.hpp"
#include "view/function/primitiveFunction.hpp"

using clang::ast_matchers::anyOf;
using clang::ast_matchers::autoType;
using clang::ast_matchers::builtinType;
using clang::ast_matchers::functionDecl;
using clang::ast_matchers::hasDeducedType;
using clang::ast_matchers::isExpansionInFileMatching;
using clang::ast_matchers::returns;
using clang::ast_matchers::internal::Matcher;

using kodgen::match::parms;
using kodgen::match::PrimitiveFunctionMatcher;

using kodgen::view::PrimitiveFunction;
using kodgen::view::PrimitiveType;
using kodgen::view::VarDecl;

kodgen::match::PrimitiveFunctionMatcher::PrimitiveFunctionMatcher(
	std::shared_ptr<MatchContext> context)
	: MatcherBase(std::move(context)) {}

auto PrimitiveFunctionMatcher::getMatcher() const
	-> clang::ast_matchers::internal::Matcher<clang::Decl> {
	Matcher<clang::QualType> primitive = anyOf(  //
		autoType(hasDeducedType(builtinType())),
		builtinType()  //
	);

	return functionDecl(  //
			   isExpansionInFileMatching("sample.cpp"),
			   parms(primitive),
			   returns(primitive)  //
			   )
		.bind("func");
}

void PrimitiveFunctionMatcher::run(
	const clang::ast_matchers::MatchFinder::MatchResult& result  //
) {
	const auto* node = result.Nodes.getNodeAs<clang::FunctionDecl>("func");

	if (node == nullptr) {
		return;
	}

	node->dump();

	auto parms = std::vector<VarDecl<PrimitiveType>>();

	for (auto parm : node->parameters()) {
		parms.push_back(VarDecl(parm->getDeclName().getAsString(),
		                        PrimitiveType(parm->getType())));
	}

	auto retType = PrimitiveType(node->getReturnType());

	auto decl = PrimitiveFunction(node->getDeclName().getAsString(), retType, parms);

	context->addDeclaration(decl);
}