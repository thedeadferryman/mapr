//
// Created by karl on 25.05.22.
//

#include "sparseSourceLoc.hpp"

#include "util/collectionUtils.hpp"
#include "util/stringBuilder.hpp"

using mapr::util::CollectionUtils;
using mapr::view::SparseSourceLoc;

SparseSourceLoc::SparseSourceLoc()
	: locations {} {}

SparseSourceLoc::SparseSourceLoc(
	std::vector<std::shared_ptr<SingleSourceLoc>> locations)
	: locations(std::move(locations)) {}

auto SparseSourceLoc::str() const -> std::string {
	return util::StringBuilder("M:(")  //
		<< CollectionUtils::mapJoin(
			   locations,
			   "; ",  //
			   [](const auto& loc) { return loc->str(); }  //
			   )  //
		<< ")";
}

auto SparseSourceLoc::getLocations() const
	-> const std::vector<std::shared_ptr<SingleSourceLoc>>& {
	return locations;
}

auto SparseSourceLoc::isEmpty() const -> bool {
	return locations.empty();
}