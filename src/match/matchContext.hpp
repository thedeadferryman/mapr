//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <vector>
#include <optional>

#include "view/declBase.hpp"

namespace kodgen::match {

class MatchContext {
	std::vector<std::shared_ptr<view::DeclBase>> declarations;

  public:
	void addDeclaration(std::shared_ptr<view::DeclBase> decl);

	[[nodiscard]] auto findDeclaration(const std::string_view &type) -> std::optional<view::DeclBase>;

	const std::vector<std::shared_ptr<view::DeclBase>>& getDeclarations() const;
};

}  // namespace kodgen::match