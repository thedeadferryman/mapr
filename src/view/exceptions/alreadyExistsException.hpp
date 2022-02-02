//
// Created by kmeinkopf on 24.12.2021.
//

#pragma once

#include "view/declBase.hpp"

#include "util/exceptions/baseException.hpp"

namespace kodgen::view {

class AlreadyExistsException : public util::BaseException {
  public:
	AlreadyExistsException(std::string_view msg, view::DeclType type);
};

}  // namespace kodgen::view