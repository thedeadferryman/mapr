//
// Created by kmeinkopf on 15.01.2022.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-macro-usage"

#include "stringBuilder.hpp"

#define OPERATOR_FOR(_, __, _Type) \
	auto StringBuilder::operator<<(_Type part)->StringBuilder& { \
		builder << part; \
		return *this; \
	}

using mapr::util::StringBuilder;

StringBuilder::StringBuilder() = default;

StringBuilder::StringBuilder(std::string_view firstPart)
	: builder() {
	builder << firstPart;
}

StringBuilder::operator std::string() const {
	return builder.str();
}

#include "util/stringBuilderSupportedTypes.inc"

#pragma clang diagnostic pop