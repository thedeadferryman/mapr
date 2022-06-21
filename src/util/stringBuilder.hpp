//
// Created by kmeinkopf on 15.01.2022.
//

#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-macro-usage"

#include <cstdint>
#include <sstream>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#define OPERATOR_FOR(_, __, _Type) auto operator<<(_Type part)->StringBuilder&;

namespace mapr::util {

class StringBuilder {
	std::stringstream builder;

  public:
	StringBuilder();

	explicit StringBuilder(std::string_view firstPart);

	operator std::string() const;

#include "util/stringBuilderSupportedTypes.inc"  // Define supported operators
};

}  // namespace mapr::util

#undef OPERATOR_FOR

#pragma clang diagnostic pop