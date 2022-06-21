//
// Created by karl on 22.03.22.
//

#pragma once

#include "util/exceptions/baseException.hpp"

namespace mapr::app {

class CommandLineException : public util::BaseException {
  public:
	explicit CommandLineException(std::string_view message);
};

}  // namespace mapr::app


