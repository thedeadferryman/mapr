//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <vector>
#include <optional>

#include "view/declBase.hpp"

namespace kodgen::match {

class MatchContext {
	std::vector<view::DeclBase> declarations;

  public:
	void addDeclaration(const view::DeclBase& decl);

	[[nodiscard]] auto findDeclaration(const std::string_view &type) -> std::optional<view::DeclBase>;

	const std::vector<view::DeclBase>& getDeclarations() const;
};

}  // namespace kodgen::match