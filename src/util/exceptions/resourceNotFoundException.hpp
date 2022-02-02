//
// Created by kmeinkopf on 20.01.2022.
//

#pragma once

#include "util/exceptions/baseException.hpp"
#include "util/resourceLoader.hpp"

namespace kodgen::util {

class ResourceNotFoundException : public BaseException {
  public:
	ResourceNotFoundException(ResourceCategory category, std::string_view name);
};

}  // namespace kodgen::util