//
// Created by karl on 25.05.22.
//

#pragma once

#include <optional>
#include <regex>

#include "view/loc/singleSourceLoc.hpp"
#include "view/loc/sourceLoc.hpp"

#include "filterBase.hpp"

namespace mapr::config {

class FilenameFilter : public FilterBase {
  public:
	enum class Mode {
		BaseName,
		AbsolutePath,
		RelativePath
	};

	[[nodiscard]] static auto matchModeFromString(std::string_view matchMode)
		-> std::optional<Mode>;

  private:
	std::regex regex;
	Mode matchMode;

  public:
	FilenameFilter(FilterMode mode, Mode matchMode, std::regex  regex);

	[[nodiscard]] auto checkDecl(const std::shared_ptr<const view::DeclBase>& decl)
		const -> FilterResult override;

	[[nodiscard]] auto checkFunctionDecl(
		const std::shared_ptr<const view::FunctionDecl>& decl) const
		-> FilterResult override;

	auto filterFunctionDecl(
		const std::shared_ptr<const view::FunctionDecl>& decl,
		SparseFilteringMode filteringMode) const
		-> std::shared_ptr<const view::FunctionDecl> override;

  private:
	[[nodiscard]] auto getFilenameForDecl(
		const std::shared_ptr<view::SingleSourceLoc>& loc) const
		-> std::optional<std::string>;

	[[nodiscard]] auto checkLoc(
		const std::shared_ptr<view::SingleSourceLoc>& loc) const
		-> bool;
};

}  // namespace mapr::config
