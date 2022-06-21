//
// Created by kmeinkopf on 24.01.2022.
//

#pragma once

#include <memory>

#include "view/types/typeBase.hpp"

#include "transform/dependencies/dependencyRequest.hpp"

namespace mapr::transform {

class TypeRequest : public DependencyRequest {
	std::shared_ptr<view::TypeBase> type;

  public:
	TypeRequest(std::shared_ptr<view::TypeBase> type);

	[[nodiscard]] auto getType() const -> std::shared_ptr<view::TypeBase>;
};

}  // namespace mapr::getName