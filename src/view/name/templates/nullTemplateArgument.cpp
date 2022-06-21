//
// Created by karl on 13.05.22.
//

#include <string>

#include "nullTemplateArgument.hpp"

using mapr::view::NullTemplateArgument;

auto NullTemplateArgument::str() const -> std::string {
	return "_";
}
