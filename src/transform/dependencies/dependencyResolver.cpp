//
// Created by kmeinkopf on 19.01.2022.
//

#include <utility>

#include "dependencyResolver.hpp"

#include "transform/aux/includeFile.hpp"
#include "transform/aux/templateFile.hpp"
#include "transform/aux/typeDecl.hpp"
#include "transform/dependencies/declRequest.hpp"
#include "transform/dependencies/typeRequest.hpp"

#include "util/matchType.hpp"

using mapr::transform::DependencyResolver;
using mapr::util::MatchType;

DependencyResolver::DependencyResolver(
	std::shared_ptr<view::DeclContext> declContext,
	std::shared_ptr<config::PipelineContext> context,
	std::shared_ptr<util::ResourceLoader> loader)
	: declContext(std::move(declContext))
	, context(std::move(context))
	, loader(std::move(loader)) {}

auto DependencyResolver::resolve(std::shared_ptr<DependencyRequest> request)
	const -> std::shared_ptr<const view::DeclBase> {
	return MatchType::matchType<std::shared_ptr<const view::DeclBase>>(
		std::move(request),  //
		[*this](const std::shared_ptr<DeclRequest>& declRequest)
			-> std::shared_ptr<const view::DeclBase> {
			return declContext->findDeclaration(
				declRequest->getDeclarationId());
		},
		[*this](const std::shared_ptr<AuxRequest>& auxRequest)
			-> std::shared_ptr<const view::DeclBase> {
			return resolveAux(auxRequest->getDependencyId());
		},
		[*this](const std::shared_ptr<TypeRequest>& typeRequest)
			-> std::shared_ptr<const view::DeclBase> {
			return std::make_shared<TypeDecl>(typeRequest->getType());
		},
		[](const std::shared_ptr<DependencyRequest>& /*unused*/)
			-> std::shared_ptr<const view::DeclBase> { UNREACHABLE(); });
}

auto DependencyResolver::resolveAux(AuxDependencyId dependencyId) const
	-> std::shared_ptr<const AuxDecl> {
	switch (dependencyId) {
		case AuxDependencyId::OverloadPrelude:
			return std::make_shared<TemplateFile>(loader,
			                                      "overloadPrelude.hpp");
		case AuxDependencyId::ExternPrelude:
			return std::make_shared<TemplateFile>(loader, "externPrelude.hpp");
		case AuxDependencyId::DeclarationsHeader:
			return std::make_shared<IncludeFile>(context->getHeaderName());
		default:
			UNREACHABLE();
	}
}
