//
// Created by karl on 26.05.22.
//

#include "filterChain.hpp"

using mapr::app::FilterChain;
using mapr::config::FilterResult;

template<typename V>
class IsEqual {
	V sample;

  public:
	explicit IsEqual(V sample);

	[[nodiscard]] auto operator()(V value) -> bool;
};

[[nodiscard]] auto anyResultEquals(const std::vector<FilterResult>& results,
                                   FilterResult sample) -> bool;

[[nodiscard]] auto noResultEquals(const std::vector<FilterResult>& results,
                                  FilterResult sample) -> bool;

FilterChain::FilterChain(FilterChain::Filters filters)
	: filters(std::move(filters)) {}

auto FilterChain::checkDecl(
	const std::shared_ptr<const view::DeclBase>& decl) const -> FilterResult {
	auto results = std::vector<FilterResult>();

	std::transform(
		std::begin(filters),
		std::end(filters),
		std::back_inserter(results),
		[decl](const auto& filter) { return decl->acceptFilter(filter); }  //
	);

	if (anyResultEquals(results, FilterResult::Exclude)) {
		return FilterResult::Exclude;
	}

	if (anyResultEquals(results, FilterResult::Include)) {
		if (noResultEquals(results, FilterResult::SparseExclude)) {
			return FilterResult::Include;
		}
		return FilterResult::SparseExclude;
	}

	if (anyResultEquals(results, FilterResult::SparseInclude)) {
		return FilterResult::SparseInclude;
	}

	return FilterResult::Ignore;
}

auto FilterChain::filterSparseDecl(
	const std::shared_ptr<const view::DeclBase>& sparseDecl,
	config::SparseFilteringMode filteringMode) const
	-> std::shared_ptr<const view::DeclBase> {
	auto current = sparseDecl;

	for (const auto& filter : filters) {
		current = current->acceptSparseFilter(filter, filteringMode);
	}

	return current;
}

auto anyResultEquals(const std::vector<FilterResult>& results,
                     FilterResult sample) -> bool {
	return std::any_of(  //
		std::begin(results),
		std::end(results),
		IsEqual(sample)  //
	);
}

auto noResultEquals(const std::vector<FilterResult>& results,
                    FilterResult sample) -> bool {
	return std::none_of(  //
		std::begin(results),
		std::end(results),
		IsEqual(sample)  //
	);
}

template<typename V>
IsEqual<V>::IsEqual(V sample)
	: sample(sample) {}

template<typename V>
auto IsEqual<V>::operator()(V value) -> bool {
	return value == sample;
}
