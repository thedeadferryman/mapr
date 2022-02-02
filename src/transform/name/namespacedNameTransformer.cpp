//
// Created by kmeinkopf on 02.02.2022.
//

#include "namespacedNameTransformer.hpp"

using kodgen::transform::NamespacedNameTransformer;

auto NamespacedNameTransformer::getName(const view::NamespacedName& name)
	-> std::string {
	return "CC_"  // TODO: Make configurable prefix
		+ boost::algorithm::join(name.getFragments(), "_");
}
