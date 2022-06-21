//
// Created by kmeinkopf on 07.01.2022.
//

#include <utility>

#include "enumMatcher.hpp"

#include "match/nameResolver.hpp"
#include "match/typeResolver.hpp"

#include "view/enum/enumDecl.hpp"
#include "view/loc/singleSourceLoc.hpp"
#include "view/types/aliasType.hpp"

#include "util/stringBuilder.hpp"

using clang::ast_matchers::enumDecl;
using clang::ast_matchers::hasDeclaration;
using clang::ast_matchers::hasType;
using clang::ast_matchers::MatchFinder;
using clang::ast_matchers::qualType;
using clang::ast_matchers::typedefNameDecl;

using mapr::match::EnumMatcher;
using mapr::match::NameResolver;
using mapr::match::TypeResolver;

using mapr::view::AliasType;
using mapr::view::EnumDecl;
using mapr::view::EnumType;
using mapr::view::SingleSourceLoc;
using mapr::view::TypeBase;

EnumMatcher::EnumMatcher(std::shared_ptr<view::DeclContext> context)
	: MatcherBase(std::move(context)) {}

void EnumMatcher::bind(MatchFinder* matchFinder) {
	const auto enumMatcher = enumDecl().bind("enum");
	const auto aliasedEnumMatcher =
		typedefNameDecl(
			hasType(qualType(hasDeclaration(enumDecl().bind("aliasedEnum")))))
			.bind("alias");

	matchFinder->addMatcher(enumMatcher, this);
	matchFinder->addMatcher(aliasedEnumMatcher, this);
}

static constexpr auto valueRadix = 16U;

auto getEnumValue(const clang::EnumConstantDecl* decl) -> std::string {
	return toString(
		decl->getInitVal(), valueRadix, decl->getInitVal().isSigned(), true);
}

struct EnumWithType {
	const clang::EnumDecl* decl;
	const std::shared_ptr<TypeBase> type;
};

[[nodiscard]] auto resolveEnumWithType(const MatchFinder::MatchResult& result)
	-> std::optional<EnumWithType> {
	if (const auto* pEnumDecl =
	        result.Nodes.getNodeAs<clang::EnumDecl>("enum")) {
		auto assocType = TypeResolver::resolve(
			result.Context->getEnumType(pEnumDecl), result.Context);

		if (auto enumType = std::dynamic_pointer_cast<EnumType>(assocType)) {
			if (enumType->getName().isValid()) {
				return EnumWithType {.decl = pEnumDecl, .type = assocType};
			}
		}
	}

	const auto* pAliasedDecl =
		result.Nodes.getNodeAs<clang::EnumDecl>("aliasedEnum");
	const auto* pAlias =
		result.Nodes.getNodeAs<clang::TypedefNameDecl>("alias");

	if (pAliasedDecl != nullptr && pAlias != nullptr) {
		const auto& resolvedAliasType = TypeResolver::resolve(
			result.Context->getTypedefType(pAlias), result.Context);
		const auto& resolvedEnumType = TypeResolver::resolve(
			result.Context->getEnumType(pAliasedDecl), result.Context);

		auto enumType = std::dynamic_pointer_cast<EnumType>(resolvedEnumType);

		if (enumType == nullptr || enumType->getName().isValid()) {
			return std::nullopt;
		}

		if (auto aliasType =
		        std::dynamic_pointer_cast<AliasType>(resolvedAliasType)) {
			if (aliasType->getName().isValid()) {
				enumType->setName(
					aliasType->getName());  // enum now takes alias name

				return EnumWithType {.decl = pAliasedDecl, .type = enumType};
			}
		}
	}

	return std::nullopt;
}

void EnumMatcher::run(const MatchFinder::MatchResult& result) {
	auto maybeType = resolveEnumWithType(result);

	if (!maybeType.has_value()) {
		return;
	}

	auto [enumNode, assocType] = maybeType.value();

	if (auto enumType = std::dynamic_pointer_cast<view::EnumType>(assocType)) {
		auto loc = SingleSourceLoc::fromClangLoc(  //
			clang::FullSourceLoc(enumNode->getBeginLoc(),
		                         *result.SourceManager)  //
		);

		auto decl = std::make_shared<EnumDecl>(enumType, loc);

		for (auto* memberDecl : enumNode->enumerators()) {
			auto memberLoc = SingleSourceLoc::fromClangLoc(  //
				clang::FullSourceLoc(enumNode->getBeginLoc(),
			                         *result.SourceManager)  //
			);

			const std::shared_ptr<EnumDecl::Member>& member =
				std::make_shared<EnumDecl::Member>(
					NameResolver::resolveNamedDeclName(  //
						memberDecl,
						result.Context  //
						),
					decl,
					getEnumValue(memberDecl),
					std::move(memberLoc)  //
				);
			decl->addMember(member);
		}

		context->addDeclaration(decl);
	} else {
		UNREACHABLE();
	}
}
