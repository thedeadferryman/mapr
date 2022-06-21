//
// Created by karl on 22.03.22.
//

#include "invalidConfigException.hpp"

#include "util/stringBuilder.hpp"

using mapr::config::InvalidConfigException;

auto InvalidConfigException::composeMessage(
	std::string_view message, const InvalidConfigException::Location& location)
	-> std::string {
	auto builder = util::StringBuilder();
	builder << message << " ( file: " << location.file << ":"
			<< location.line  //
			<< ":" << location.column;

	if (location.property != std::nullopt) {
		builder << " ; property: " << location.property.value();
	}

	return builder << " )";
}

InvalidConfigException::InvalidConfigException(
	std::string_view message, const InvalidConfigException::Location& location)
	: BaseException(composeMessage(message, (location))) {}
