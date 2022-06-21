//
// Created by kmeinkopf on 26.12.2021.
//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "view/name/templates/templateArgumentBase.hpp"

#include "view/name/nameFragment.hpp"

namespace mapr::view {

class QualifiedName {
	std::vector<NameFragment> fragments;

	explicit QualifiedName(std::vector<NameFragment> fragments);

  public:
	QualifiedName();

	explicit QualifiedName(std::string_view name, NameFragment::TemplateArgumentList templateArgs = {});

	explicit QualifiedName(NameFragment name);

	[[nodiscard]] auto str() const -> std::string;

	[[nodiscard]] auto parent() const -> QualifiedName;

	[[nodiscard]] auto basename() const -> QualifiedName;

	[[nodiscard]] auto operator/(std::string_view part) const -> QualifiedName;

	[[nodiscard]] auto operator/(const QualifiedName& other) const
		-> QualifiedName;

	[[nodiscard]] auto getFragments() const -> const std::vector<NameFragment>&;

	[[nodiscard]] auto isValid() const -> bool;
};

}  // namespace mapr::view