//
// Created by kmeinkopf on 24.12.2021.
//

#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace mapr::util {

class BaseException : public std::runtime_error {
  public:
	explicit BaseException(std::string_view message);
};
}  // namespace mapr::util