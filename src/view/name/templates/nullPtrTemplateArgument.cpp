//
// Created by karl on 13.05.22.
//

#include <string>

#include "nullPtrTemplateArgument.hpp"

using mapr::view::NullPtrTemplateArgument;

auto NullPtrTemplateArgument::str() const -> std::string {
	return "null";
}