//
// Created by kmeinkopf on 15.12.2021.
//

#include "recordMatcher.hpp"

#include "match/matchers/internal/recordHandler.hpp"
#include "match/nameResolver.hpp"
#include "match/overloadMismatchException.hpp"

#include "view/name/templates/integralTemplateArgument.hpp"

using mapr::match::NameResolver;
using mapr::match::RecordMatcher;

using mapr::view::QualifiedName;
using mapr::view::RecordDecl;

using clang::ast_matchers::allOf;
using clang::ast_matchers::anyOf;
using clang::ast_matchers::anything;
using clang::ast_matchers::hasDeclaration;
using clang::ast_matchers::hasType;
using clang::ast_matchers::isDefinition;
using clang::ast_matchers::isStruct;
using clang::ast_matchers::isUnion;
using clang::ast_matchers::qualType;
using clang::ast_matchers::recordDecl;
using clang::ast_matchers::typedefNameDecl;
using clang::ast_matchers::unless;

using clang::CXXMethodDecl;
using clang::CXXRecordDecl;

struct RecordDeclWithName {  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	const clang::CXXRecordDecl* recordDecl = nullptr;
	QualifiedName qualName;
} __attribute__((aligned(32)));

auto getRecordWithName(
	const clang::ast_matchers::MatchFinder::MatchResult& result)
	-> std::optional<RecordDeclWithName>;

RecordMatcher::RecordMatcher(std::shared_ptr<view::DeclContext> context)
	: MatcherBase(std::move(context)) {}

void RecordMatcher::bind(clang::ast_matchers::MatchFinder* matchFinder) {
	auto recordMatcher = allOf(  //
		isDefinition(),  // is not a forward-declaration
		unless(isUnion()));

	const auto explicitClassMatcher = recordDecl(recordMatcher).bind("record");

	const auto aliasedClassMatcher =
		typedefNameDecl(hasType(qualType(hasDeclaration(
							recordDecl(recordMatcher).bind("aliasedRecord")))))
			.bind("alias");

	matchFinder->addMatcher(aliasedClassMatcher, this);
	matchFinder->addMatcher(explicitClassMatcher, this);
}

void RecordMatcher::run(
	const clang::ast_matchers::MatchFinder::MatchResult& result) {
	auto maybeRecord = getRecordWithName(result);

	if (!maybeRecord.has_value()) {
		return;
	}
	auto handler = RecordHandler(result.Context, result.SourceManager, context);
	auto [pRecordDecl, qualName] = maybeRecord.value();

	handler.handle(pRecordDecl, qualName);
}

auto getRecordWithName(
	const clang::ast_matchers::MatchFinder::MatchResult& result)
	-> std::optional<RecordDeclWithName> {
	if (const auto* pRecordDecl =
	        result.Nodes.getNodeAs<CXXRecordDecl>("record")) {
		auto qualName =
			NameResolver::resolveDeclName(pRecordDecl, result.Context);

		if (qualName.isValid()) {
			return RecordDeclWithName {.recordDecl = pRecordDecl,
			                           .qualName = qualName};
		}
	}

	const auto* pRecordDecl =
		result.Nodes.getNodeAs<CXXRecordDecl>("aliasedRecord");
	const auto* pAliasDecl =
		result.Nodes.getNodeAs<clang::TypedefNameDecl>("alias");

	if (pRecordDecl != nullptr && pAliasDecl != nullptr) {
		auto originalName =
			NameResolver::resolveDeclName(pRecordDecl, result.Context);
		auto qualName =
			NameResolver::resolveNamedDeclName(pAliasDecl, result.Context);

		if (originalName.isValid()) {
			return std::nullopt;
		}

		if (qualName.isValid()) {
			return RecordDeclWithName {.recordDecl = pRecordDecl,
			                           .qualName = qualName};
		}
	}

	return std::nullopt;
}
