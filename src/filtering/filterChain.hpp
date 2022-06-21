//
// Created by karl on 26.05.22.
//

#pragma once

#include <vector>

#include "view/sparseDecl.hpp"

#include "filtering/filters/filterBase.hpp"

namespace mapr::app {

class FilterChain {
	using Filters = std::vector<std::shared_ptr<const config::FilterBase>>;

	Filters filters;

  public:
	explicit FilterChain(Filters filters);

	[[nodiscard]] auto checkDecl(
		const std::shared_ptr<const view::DeclBase>& decl) const
		-> config::FilterResult;

	[[nodiscard]] auto filterSparseDecl(
		const std::shared_ptr<const view::DeclBase>& sparseDecl,
		config::SparseFilteringMode filteringMode) const
		-> std::shared_ptr<const view::DeclBase>;
};

}  // namespace mapr::app