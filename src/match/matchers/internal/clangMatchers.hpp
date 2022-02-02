//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <iostream>

#include <clang/ASTMatchers/ASTMatchers.h>

namespace kodgen::match {

/// Credited from Clang 14 code
///
/// Matches a function declared with the specified return `TypeLoc`.
///
/// Given
/// \code
///   int f() { return 5; }
///   void g() {}
/// \endcode
/// functionDecl(hasReturnTypeLoc(loc(asString("int"))))
///   matches the declaration of `f`, but not `g`.
AST_MATCHER_P(clang::FunctionDecl,
              parms,
              clang::ast_matchers::internal::Matcher<clang::QualType>,
              TypeMatcher) {
	auto success = true;

	for (auto* parm : Node.parameters()) {
		success =
			success and TypeMatcher.matches(parm->getType(), Finder, Builder);
	}

	return success;
}

}  // namespace kodgen::match