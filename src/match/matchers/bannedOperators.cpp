//
// Created by karl on 22.05.22.
//

#include <algorithm>
#include <initializer_list>

#include "bannedOperators.hpp"

using clang::OverloadedOperatorKind;

namespace mapr::match {

static constexpr auto BannedOperators = {
	clang::OO_New,
	clang::OO_Array_New,
	clang::OO_Delete,
	clang::OO_Array_Delete,
};

auto isBannedOperator(clang::OverloadedOperatorKind kind) -> bool {
	return std::any_of(std::begin(BannedOperators),
	                   std::end(BannedOperators),
	                   [kind](OverloadedOperatorKind bannedKind) {
						   return kind == bannedKind;
					   });
}

}  // namespace mapr::match