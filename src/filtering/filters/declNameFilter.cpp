//
// Created by karl on 25.05.22.
//

#include <utility>

#include "declNameFilter.hpp"

#include "view/declBase.hpp"
#include "view/function/functionDecl.hpp"

using mapr::config::DeclNameFilter;

DeclNameFilter::DeclNameFilter(FilterMode mode,
                               DeclNameFilter::Mode matchMode,
                               std::regex regex)
	: FilterBase(mode)
	, matchMode(matchMode)
	, regex(std::move(regex)) {}

auto DeclNameFilter::checkDecl(
	const std::shared_ptr<const view::DeclBase>& decl) const -> FilterResult {
	if (std::regex_match(getNameForDecl(decl), regex)) {
		switch (getMode()) {
			case FilterMode::Include:
				return FilterResult::Include;
			case FilterMode::Exclude:
				return FilterResult::Exclude;
		}
	} else {
		return FilterResult::Ignore;
	}
}

auto DeclNameFilter::checkFunctionDecl(
	const std::shared_ptr<const view::FunctionDecl>& decl) const
	-> FilterResult {
	return checkDecl(decl);
}

auto DeclNameFilter::getNameForDecl(
	const std::shared_ptr<const view::DeclBase>& decl) const -> std::string {
	switch (matchMode) {
		case Mode::BaseName:
			return decl->getQualifiedName().basename().str();
		case Mode::QualName:
			return decl->getQualifiedName().str();
		default:
			UNREACHABLE();
	}
}

auto DeclNameFilter::matchModeFromString(std::string_view matchMode)
	-> std::optional<Mode> {
	if (matchMode == "basename") {
		return Mode::BaseName;
	}
	if (matchMode == "qualname") {
		return Mode::QualName;
	}

	return std::nullopt;
}

auto DeclNameFilter::filterFunctionDecl(
	const std::shared_ptr<const view::FunctionDecl>& decl,
	SparseFilteringMode filteringMode) const
	-> std::shared_ptr<const view::FunctionDecl> {
	return decl->clone();
}
