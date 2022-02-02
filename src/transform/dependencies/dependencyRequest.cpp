//
// Created by kmeinkopf on 24.01.2022.
//

#include "dependencyRequest.hpp"

using kodgen::transform::DependencyRequest;

DependencyRequest::DependencyRequest(DependencyKind kind)
	: kind(kind) {}

auto DependencyRequest::getKind() const -> DependencyKind {
	return kind;
}
