//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include <boost/algorithm/string/join.hpp>

#include "view/namespacedName.hpp"

namespace kodgen::transform {

class NamespacedNameTransformer {
  public:
	[[nodiscard]] static auto getName(const view::NamespacedName& name)
		-> std::string;
};

}  // namespace kodgen::getName