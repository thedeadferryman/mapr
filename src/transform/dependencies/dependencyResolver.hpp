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

namespace kodgen::transform {

class DependencyResolver {
	std::shared_ptr<view::DeclContext> context;
	util::ResourceLoader loader;

  public:
	explicit DependencyResolver(std::shared_ptr<view::DeclContext> context,
	                            util::ResourceLoader loader);

	[[nodiscard]] auto resolve(std::shared_ptr<DependencyRequest> request) const
		-> std::shared_ptr<view::DeclBase>;

  private:
	[[nodiscard]] auto resolveAux(AuxDependencyId dependencyId) const
		-> std::shared_ptr<AuxDecl>;
};

}  // namespace kodgen::getName
