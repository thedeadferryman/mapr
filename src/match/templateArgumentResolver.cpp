//
// Created by karl on 13.05.22.
//

#include "templateArgumentResolver.hpp"

#include <clang/AST/ASTContext.h>

#include "view/name/templates/integralTemplateArgument.hpp"
#include "view/name/templates/nullPtrTemplateArgument.hpp"
#include "view/name/templates/nullTemplateArgument.hpp"
#include "view/name/templates/typeTemplateArgument.hpp"

#include "match/typeResolver.hpp"

using mapr::match::TemplateArgumentResolver;

auto TemplateArgumentResolver::forClangArgument(
	clang::TemplateArgument argument, const clang::ASTContext* context)
	-> std::shared_ptr<view::TemplateArgumentBase> {
	switch (argument.getKind()) {
		case clang::TemplateArgument::Integral:
			return std::make_shared<view::IntegralTemplateArgument>(
				argument.getAsIntegral());
		case clang::TemplateArgument::Null:
			return std::make_shared<view::NullTemplateArgument>();
		case clang::TemplateArgument::NullPtr:
			return std::make_shared<view::NullPtrTemplateArgument>();
		case clang::TemplateArgument::Type:
			return std::make_shared<view::TypeTemplateArgument>(
				TypeResolver::resolve(argument.getAsType(), context));
		case clang::TemplateArgument::Declaration:
		case clang::TemplateArgument::Template:
		case clang::TemplateArgument::TemplateExpansion:
		case clang::TemplateArgument::Expression:
		case clang::TemplateArgument::Pack:
//			llvm::errs() << "\n--------------- ERROR RECOGNIZING ARGUMENT!\n";
//			argument.dump();
//			llvm::errs() << "\n--------------- END ERROR\n";
			return nullptr;
	}
}