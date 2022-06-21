//
// Created by kmeinkopf on 24.01.2022.
//

#include "dependencyRequest.hpp"

using mapr::transform::DependencyRequest;

DependencyRequest::DependencyRequest(DependencyKind kind)
	: kind(kind)
	, retention(DependencyRetention::Instant) {}

auto DependencyRequest::getKind() const -> DependencyKind {
	return kind;
}

auto DependencyRequest::getRetention() const -> DependencyRetention {
	return retention;
}

void DependencyRequest::setRetention(DependencyRetention dependencyRetention) {
	retention = dependencyRetention;
}
