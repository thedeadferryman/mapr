//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include "transform/dependencies/dependencyResolver.hpp"

#include "util/exceptions/baseException.hpp"

namespace mapr::transform {

class UndefinedDeclException : public util::BaseException {
  public:
	static auto fromRequest(const std::shared_ptr<DependencyRequest>& typeRequest)
		-> UndefinedDeclException;

	explicit UndefinedDeclException(std::string_view message);
};

}  // namespace mapr::getName
