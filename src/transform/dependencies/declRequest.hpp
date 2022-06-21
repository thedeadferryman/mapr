//
// Created by kmeinkopf on 24.01.2022.
//

#pragma once

#include <string>

#include "transform/dependencies/dependencyRequest.hpp"

namespace mapr::transform {

class DeclRequest : public DependencyRequest {
	std::string declId;

  public:
	DeclRequest(std::string_view declId);

	[[nodiscard]] auto getDeclarationId() const -> const std::string&;
};

}  // namespace mapr::getName