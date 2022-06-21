//
// Created by karl on 25.05.22.
//

#pragma once

#include <memory>

#include "util/macro.hpp"

namespace mapr::view {

class DeclBase;
class FunctionDecl;

}  // namespace mapr::view

namespace mapr::config {

S_ENUM(FilterResult,
       Include,  // force declaration processing
       SparseInclude,  // ... for 'SparseDecl'-s
       Exclude,  // skip declaration processing
       SparseExclude,  // ... for 'SparseDecl'-s
       Ignore  // ignore filter result
);

enum class FilterMode {
	Include,
	Exclude
};

enum class SparseFilteringMode {
	Include,
	Exclude
};

[[nodiscard]] auto isSparseResult(FilterResult result) -> bool;
[[nodiscard]] auto sparseResultToFilteringMode(FilterResult result)
	-> SparseFilteringMode;

class FilterBase {
	FilterMode mode;

  public:
	explicit FilterBase(FilterMode mode);

	[[nodiscard]] virtual auto checkDecl(
		const std::shared_ptr<const view::DeclBase>& decl) const
		-> FilterResult = 0;

	[[nodiscard]] virtual auto checkFunctionDecl(
		const std::shared_ptr<const view::FunctionDecl>& decl) const
		-> FilterResult = 0;

	[[nodiscard]] virtual auto filterFunctionDecl(
		const std::shared_ptr<const view::FunctionDecl>& decl,
		SparseFilteringMode filteringMode) const
		-> std::shared_ptr<const view::FunctionDecl> = 0;

	[[nodiscard]] virtual auto getMode() const -> FilterMode;

	FilterBase(const FilterBase&) = delete;
	FilterBase(FilterBase&&) = delete;
	auto operator=(const FilterBase&) -> FilterBase& = delete;
	auto operator=(FilterBase&&) -> FilterBase& = delete;
	virtual ~FilterBase() = default;
};

}  // namespace mapr::config
