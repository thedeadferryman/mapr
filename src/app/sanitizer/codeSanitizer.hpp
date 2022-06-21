//
// Created by karl on 22.03.22.
//

#pragma once

#include "config/toolProperties.hpp"

namespace mapr::app {

class CodeSanitizer {
	config::FormatOptions options;

  public:
	explicit CodeSanitizer(config::FormatOptions options);

	void sanitize(std::string_view code, std::ostream &output);
};

}  // namespace mapr::app


