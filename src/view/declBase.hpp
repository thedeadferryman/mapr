//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <memory>
#include <string>

#include "util/macro.hpp"

namespace kodgen::view {

S_ENUM(DeclType,
       Base,
       Type,
       Typedef,
       Enum,
       EnumMember,
       Var,
       Function,
       FunctionOverload,
       Internal
       )

class DeclBase {
	const std::string id;

	const DeclType declType;

  public:
	explicit DeclBase(std::string_view id, DeclType declType);

	[[nodiscard]] virtual auto getID() const -> const std::string&;

	[[nodiscard]] auto getDeclType() const -> DeclType;

	DeclBase(const DeclBase&) = delete;
	DeclBase(DeclBase&&) = delete;
	auto operator=(const DeclBase&) -> DeclBase& = delete;
	auto operator=(DeclBase&&) -> DeclBase& = delete;
	virtual ~DeclBase() = default;
};

}  // namespace kodgen::view