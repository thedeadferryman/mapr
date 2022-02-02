//
// Created by kmeinkopf on 26.12.2021.
//

#include <optional>

#include "nameResolver.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>

using kodgen::match::NameResolver;
using kodgen::view::NamespacedName;

auto NameResolver::resolveDeclName(const clang::DeclContext* decl)
	-> view::NamespacedName {
	auto fqn = NamespacedName();

	auto* current = decl;

	while (current != nullptr) {
		if (const auto* namedDecl = llvm::dyn_cast<clang::NamedDecl>(current)) {
			fqn = NamespacedName(namedDecl->getNameAsString()) / fqn;
			current = current->getParent();
		} else {
			fqn = NamespacedName("") / fqn;
			break;
		}
	}

	return fqn;
}
