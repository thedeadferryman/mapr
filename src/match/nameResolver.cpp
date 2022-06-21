//
// Created by kmeinkopf on 26.12.2021.
//

#include <optional>

#include "nameResolver.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclTemplate.h>

#include "match/templateArgumentResolver.hpp"

using mapr::match::NameResolver;
using mapr::match::TemplateArgumentResolver;
using mapr::view::QualifiedName;

using TemplateArgumentList = mapr::view::NameFragment::TemplateArgumentList;

auto resolveFragmentForDecl(const clang::NamedDecl* decl,
                            const clang::ASTContext* context) {
	if (const auto* templateInst =
	        llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(decl)) {
		auto args = TemplateArgumentList();

		for (auto arg :
		     templateInst->getTemplateArgs().asArray()) {
			if (auto mappedArg =
			        TemplateArgumentResolver::forClangArgument(arg, context)) {
				args.push_back(mappedArg);
			}
		}

		return QualifiedName(decl->getNameAsString(), args);
	}

	return QualifiedName(decl->getNameAsString());
}

auto NameResolver::resolveDeclName(const clang::DeclContext* decl,
                                   const clang::ASTContext* context)
	-> view::QualifiedName {
	auto fqn = QualifiedName();

	const auto* current = decl;

	while (current != nullptr) {
		if (const auto* namedDecl = llvm::dyn_cast<clang::NamedDecl>(current)) {
			fqn =
				QualifiedName(resolveFragmentForDecl(namedDecl, context)) / fqn;
			current = current->getParent();
		} else {
			fqn = QualifiedName("") / fqn;
			break;
		}
	}

	return fqn;
}

auto NameResolver::resolveNamedDeclName(const clang::NamedDecl* decl,
                                        const clang::ASTContext* context)
	-> view::QualifiedName {
	const auto* declContext = decl->getDeclContext();

	return resolveDeclName(declContext, context)
		/ QualifiedName(resolveFragmentForDecl(decl, context));
}