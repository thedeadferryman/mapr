//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <map>
#include <optional>
#include <vector>

#include "view/declBase.hpp"

namespace kodgen::view {

class DeclContext {
	std::map<std::string, std::shared_ptr<view::DeclBase>> declarations;

  public:
	void addDeclaration(const std::shared_ptr<view::DeclBase>& decl);

	[[nodiscard]] auto findDeclaration(std::string_view id) const
		-> std::shared_ptr<view::DeclBase>;

	[[nodiscard]] auto getDeclarations() const
		-> const std::map<std::string, std::shared_ptr<view::DeclBase>>&;
};

}  // namespace kodgen::view