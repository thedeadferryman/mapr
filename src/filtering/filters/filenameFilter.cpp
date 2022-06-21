//
// Created by karl on 25.05.22.
//

#include <filesystem>
#include <utility>

#include "filenameFilter.hpp"

#include "view/declBase.hpp"
#include "view/function/functionDecl.hpp"
#include "view/loc/singleSourceLoc.hpp"

using mapr::config::FilenameFilter;
using mapr::config::FilterMode;
using mapr::config::FilterResult;

namespace FS = std::filesystem;

[[nodiscard]] auto positiveResultForMode(FilterMode mode) -> FilterResult;

[[nodiscard]] auto partialResultForMode(FilterMode mode) -> FilterResult;

auto FilenameFilter::matchModeFromString(std::string_view matchMode)
	-> std::optional<Mode> {
	if (matchMode == "abspath") {
		return Mode::AbsolutePath;
	}
	if (matchMode == "relpath") {
		return Mode::RelativePath;
	}
	if (matchMode == "basename") {
		return Mode::BaseName;
	}

	return std::nullopt;
}

FilenameFilter::FilenameFilter(FilterMode mode,
                               FilenameFilter::Mode matchMode,
                               std::regex regex)
	: FilterBase(mode)
	, regex(std::move(regex))
	, matchMode(matchMode) {}

auto FilenameFilter::checkDecl(
	const std::shared_ptr<const view::DeclBase>& decl) const -> FilterResult {
	if (auto singleLoc = std::dynamic_pointer_cast<view::SingleSourceLoc>(
			decl->getLocation())) {
		if (checkLoc(singleLoc)) {
			return positiveResultForMode(getMode());
		}
		return FilterResult::Ignore;
	}

	UNREACHABLE();
}
auto FilenameFilter::checkFunctionDecl(
	const std::shared_ptr<const view::FunctionDecl>& decl) const
	-> FilterResult {
	auto locations = decl->getSparseLocation()->getLocations();

	bool allMatch = std::all_of(  //
		std::begin(locations),
		std::end(locations),
		[this](auto loc) { return checkLoc(loc); }  //
	);

	if (allMatch) {
		return positiveResultForMode(getMode());
	}

	bool anyMatch = std::any_of(  //
		std::begin(locations),
		std::end(locations),
		[this](auto loc) { return checkLoc(loc); }  //
	);

	if (anyMatch) {
		return partialResultForMode(getMode());
	}

	return FilterResult::Ignore;
}

auto FilenameFilter::getFilenameForDecl(
	const std::shared_ptr<view::SingleSourceLoc>& loc) const
	-> std::optional<std::string> {
	if (!exists(FS::path(loc->getPath()))) {
		return std::nullopt;
	}

	switch (matchMode) {
		case Mode::BaseName:
			return FS::path(loc->getPath()).filename().c_str();
		case Mode::AbsolutePath:
			return absolute(FS::path(loc->getPath())).c_str();
		case Mode::RelativePath:
			return FS::path(loc->getPath()).c_str();
	}
}

auto FilenameFilter::checkLoc(
	const std::shared_ptr<view::SingleSourceLoc>& loc) const -> bool {
	if (auto filename = getFilenameForDecl(loc)) {
		if (std::regex_match(filename.value(), regex)) {
			return true;
		}
	}
	return false;
}

auto FilenameFilter::filterFunctionDecl(
	const std::shared_ptr<const view::FunctionDecl>& decl,
	SparseFilteringMode filteringMode  //
) const -> std::shared_ptr<const view::FunctionDecl> {
	auto newDecl = decl->clone();
	newDecl->dropOverloads();

	for (const auto& overload : decl->getOverloads()) {
		auto fitsFilter = checkLoc(overload.getLocation());

		if (getMode() == FilterMode::Exclude && fitsFilter) {
			continue;
		}

		if (filteringMode == SparseFilteringMode::Exclude
		    || (filteringMode == SparseFilteringMode::Include
		        && getMode() == FilterMode::Include && fitsFilter)) {
			newDecl->addOverload(overload);
		}
	}

	return newDecl;
}

auto positiveResultForMode(FilterMode mode) -> FilterResult {
	switch (mode) {
		case FilterMode::Include:
			return FilterResult::Include;
		case FilterMode::Exclude:
			return FilterResult::Exclude;
	}
}

auto partialResultForMode(FilterMode mode) -> FilterResult {
	switch (mode) {
		case FilterMode::Include:
			return FilterResult::SparseInclude;
		case FilterMode::Exclude:
			return FilterResult::SparseExclude;
	}
}
