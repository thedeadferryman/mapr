//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <map>
#include <optional>
#include <vector>

#include "view/declBase.hpp"

namespace mapr::view {

class DeclContext {
	std::map<std::string, std::shared_ptr<const view::DeclBase>> declarations;

  public:
	void addDeclaration(const std::shared_ptr<const view::DeclBase>& decl);

	[[nodiscard]] auto findDeclaration(std::string_view id) const
		-> std::shared_ptr<const view::DeclBase>;

	void replaceDeclaration(const std::shared_ptr<const DeclBase>& decl);

	[[nodiscard]] auto getDeclarations() const
		-> const std::map<std::string, std::shared_ptr<const view::DeclBase>>&;
};

}  // namespace mapr::view