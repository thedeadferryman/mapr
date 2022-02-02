//
// Created by kmeinkopf on 24.01.2022.
//

#pragma once

#include <variant>

#include "util/macro.hpp"

namespace kodgen::transform {

S_ENUM(DependencyKind, Type, Decl, Aux)

class DependencyRequest {
	DependencyKind kind;

  public:
	DependencyRequest(DependencyKind kind);

	DependencyRequest(const DependencyRequest&) = delete;
	DependencyRequest(DependencyRequest&&) = delete;

	auto operator=(const DependencyRequest&) -> DependencyRequest& = delete;
	auto operator=(DependencyRequest&&) -> DependencyRequest& = delete;

	virtual ~DependencyRequest() = default;

	[[nodiscard]] auto getKind() const -> DependencyKind;
};

}  // namespace kodgen::getName