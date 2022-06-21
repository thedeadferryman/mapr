//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include <variant>

#include "view/declContext.hpp"

#include "transform/aux/auxDecl.hpp"
#include "transform/dependencies/auxRequest.hpp"
#include "transform/dependencies/dependencyRequest.hpp"

#include "util/resourceLoader.hpp"

#include "config/pipelineContext.hpp"

namespace mapr::transform {

class DependencyResolver {
	std::shared_ptr<view::DeclContext> declContext;
	std::shared_ptr<config::PipelineContext> context;
	std::shared_ptr<util::ResourceLoader> loader;

  public:
	DependencyResolver(
		std::shared_ptr<view::DeclContext> declContext,
		std::shared_ptr<config::PipelineContext> context,
		std::shared_ptr<util::ResourceLoader> loader);

	[[nodiscard]] auto resolve(std::shared_ptr<DependencyRequest> request) const
		-> std::shared_ptr<const view::DeclBase>;

  private:
	[[nodiscard]] auto resolveAux(AuxDependencyId dependencyId) const
		-> std::shared_ptr<const AuxDecl>;
};

}  // namespace mapr::transform
