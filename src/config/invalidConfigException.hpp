//
// Created by karl on 22.03.22.
//

#pragma once

#include <optional>

#include "util/exceptions/baseException.hpp"

namespace mapr::config {

class InvalidConfigException : public util::BaseException {
  public:
	struct Location {
		std::string file;
		std::int32_t line;
		std::int32_t column;
		std::optional<std::string> property;
	} __attribute__((aligned(128)));

  private:
	[[nodiscard]] static auto composeMessage(std::string_view message,
	                                         const Location& location)
		-> std::string;

  public:
	InvalidConfigException(std::string_view message,
	                       const InvalidConfigException::Location& location);
};

}  // namespace mapr::app


