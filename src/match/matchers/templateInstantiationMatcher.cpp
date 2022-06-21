//
// Created by karl on 14.05.22.
//

#include "templateInstantiationMatcher.hpp"

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Sema/Template.h>

#include "match/matchers/internal/recordHandler.hpp"
#include "match/nameResolver.hpp"

using clang::ast_matchers::classTemplateSpecializationDecl;
using clang::ast_matchers::functionDecl;
using clang::ast_matchers::hasDeclaration;
using clang::ast_matchers::MatchFinder;
using clang::ast_matchers::returns;

using mapr::match::TemplateInstantiationMatcher;

TemplateInstantiationMatcher::TemplateInstantiationMatcher(
	std::shared_ptr<view::DeclContext> context,
	std::shared_ptr<clang::CompilerInstance> compiler  //
	)
	: MatcherBase(std::move(context))
	, compiler(std::move(compiler)) {}

void TemplateInstantiationMatcher::bind(MatchFinder* matchFinder) {
	auto matcher = functionDecl(  //
		returns(hasDeclaration(  //
			classTemplateSpecializationDecl().bind("templateSpecialization")  //
			))  //
	);

	matchFinder->addMatcher(matcher, this);
}

void TemplateInstantiationMatcher::run(const MatchFinder::MatchResult& result) {
	const auto* node =
		result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>(
			"templateSpecialization");

	if (node == nullptr) {
		return;
	}

	if (const auto* pRecordDecl = instantiateTemplate(node)) {
		if (pRecordDecl->isCompleteDefinition()) {
			return;
		}

		auto handler =
			RecordHandler(result.Context, result.SourceManager, context);
		auto qualName =
			NameResolver::resolveDeclName(pRecordDecl, result.Context);

		//		handler.handle(pRecordDecl, qualName);
	}
}

auto TemplateInstantiationMatcher::instantiateTemplate(
	const clang::ClassTemplateSpecializationDecl* templateSpec)
	-> const clang::CXXRecordDecl* {
	return nullptr;
}
