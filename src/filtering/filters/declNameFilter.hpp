//
// Created by karl on 25.05.22.
//

#pragma once

#include <optional>
#include <regex>

#include "filterBase.hpp"

namespace mapr::config {

class DeclNameFilter : public FilterBase {
  public:
	enum class Mode {
		BaseName,
		QualName
	};

	[[nodiscard]] static auto matchModeFromString(std::string_view matchMode)
		-> std::optional<Mode>;

  private:
	Mode matchMode;
	std::regex regex;

  public:
	DeclNameFilter(FilterMode mode, Mode matchMode, std::regex  regex);

	[[nodiscard]] auto checkDecl(const std::shared_ptr<const view::DeclBase>& decl)
		const -> FilterResult override;
	[[nodiscard]] auto checkFunctionDecl(
		const std::shared_ptr<const view::FunctionDecl>& decl) const
		-> FilterResult override;

	[[nodiscard]] auto filterFunctionDecl(
		const std::shared_ptr<const view::FunctionDecl>& decl,
		SparseFilteringMode filteringMode) const
		-> std::shared_ptr<const view::FunctionDecl> override;

  private:
	[[nodiscard]] auto getNameForDecl(
		const std::shared_ptr<const view::DeclBase>& decl) const -> std::string;
};

}  // namespace mapr::config
