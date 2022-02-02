//
// Created by kmeinkopf on 19.01.2022.
//

#include "dependencyResolver.hpp"

#include "transform/aux/templateFile.hpp"
#include "transform/aux/typeDecl.hpp"
#include "transform/dependencies/declRequest.hpp"
#include "transform/dependencies/typeRequest.hpp"

#include "util/matchType.hpp"

using kodgen::transform::DependencyResolver;
using kodgen::util::MatchType;

DependencyResolver::DependencyResolver(
	std::shared_ptr<view::DeclContext> context, util::ResourceLoader loader)
	: context(std::move(context))
	, loader(std::move(loader)) {}

auto DependencyResolver::resolve(std::shared_ptr<DependencyRequest> request)
	const -> std::shared_ptr<view::DeclBase> {
	return MatchType::matchType<std::shared_ptr<view::DeclBase>>(
		request,  //
		[=](const std::shared_ptr<DeclRequest>& declRequest)
			-> std::shared_ptr<view::DeclBase> {
			return context->findDeclaration(declRequest->getDeclarationId());
		},
		[=](const std::shared_ptr<AuxRequest>& auxRequest)
			-> std::shared_ptr<view::DeclBase> {
			return resolveAux(auxRequest->getDependencyId());
		},
		[=](const std::shared_ptr<TypeRequest>& typeRequest)
			-> std::shared_ptr<view::DeclBase> {
			return std::make_shared<TypeDecl>(typeRequest->getType());
		},
		[](const std::shared_ptr<DependencyRequest>& /*unused*/)
			-> std::shared_ptr<view::DeclBase> { UNREACHABLE(); });
}

auto DependencyResolver::resolveAux(AuxDependencyId dependencyId) const
	-> std::shared_ptr<AuxDecl> {
	switch (dependencyId) {
		case AuxDependencyId::OverloadPrelude:
			return std::make_shared<TemplateFile>(loader,
			                                      "overloadPrelude.hpp");
		case AuxDependencyId::ExternPrelude:
			return std::make_shared<TemplateFile>(loader, "externPrelude.hpp");
		default:
			UNREACHABLE();
	}
}
