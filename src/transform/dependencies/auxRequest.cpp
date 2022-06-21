//
// Created by kmeinkopf on 24.01.2022.
//

#include "auxRequest.hpp"

using mapr::transform::AuxRequest;

AuxRequest::AuxRequest(AuxDependencyId id)
	: DependencyRequest(DependencyKind::Aux)
	, dependencyId(id) {}

auto AuxRequest::getDependencyId() const -> AuxDependencyId {
	return dependencyId;
}
