//
// Created by kmeinkopf on 24.01.2022.
//

#pragma once

#include <variant>

#include "util/macro.hpp"

namespace mapr::transform {

S_ENUM(DependencyKind, Type, Decl, Aux)

enum class DependencyRetention {
	Instant,
	Deferred
};

class DependencyRequest {
	DependencyKind kind;
	DependencyRetention retention;

  public:
	explicit DependencyRequest(DependencyKind kind);

	DependencyRequest(const DependencyRequest&) = delete;
	DependencyRequest(DependencyRequest&&) = delete;

	auto operator=(const DependencyRequest&) -> DependencyRequest& = delete;
	auto operator=(DependencyRequest&&) -> DependencyRequest& = delete;

	virtual ~DependencyRequest() = default;

	[[nodiscard]] auto getKind() const -> DependencyKind;

	[[nodiscard]] auto getRetention() const -> DependencyRetention;

	void setRetention(DependencyRetention dependencyRetention);
};

}  // namespace mapr::transform