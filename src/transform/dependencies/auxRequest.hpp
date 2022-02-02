//
// Created by kmeinkopf on 24.01.2022.
//

#pragma once

#include "transform/dependencies/dependencyRequest.hpp"

namespace kodgen::transform {

S_ENUM(AuxDependencyId, OverloadPrelude, ExternPrelude);

class AuxRequest : public DependencyRequest {
	AuxDependencyId dependencyId;

  public:
	AuxRequest(AuxDependencyId id);

	[[nodiscard]] auto getDependencyId() const -> AuxDependencyId;
};

}  // namespace kodgen::getName