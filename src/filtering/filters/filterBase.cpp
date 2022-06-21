//
// Created by karl on 25.05.22.
//
#include <algorithm>

#include "filterBase.hpp"

using mapr::config::FilterBase;
using mapr::config::FilterResult;

constexpr auto SparseResults = {
	FilterResult::SparseExclude,
	FilterResult::SparseInclude,
};

auto mapr::config::isSparseResult(FilterResult result) -> bool {
	return std::find(std::begin(SparseResults), std::end(SparseResults), result)
		!= std::end(SparseResults);
}

auto mapr::config::sparseResultToFilteringMode(FilterResult result)
	-> SparseFilteringMode {
	switch (result) {
		case FilterResult::SparseInclude:
			return SparseFilteringMode::Include;
		case FilterResult::SparseExclude:
			return SparseFilteringMode::Exclude;
		default:
			UNREACHABLE();
	}
}

FilterBase::FilterBase(FilterMode mode)
	: mode(mode) {}

auto FilterBase::getMode() const -> FilterMode {
	return mode;
}