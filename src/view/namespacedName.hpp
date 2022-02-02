//
// Created by kmeinkopf on 26.12.2021.
//

#pragma once

#include <string>
#include <vector>

namespace kodgen::view {

class NamespacedName {
	std::vector<std::string> fragments;

	explicit NamespacedName(std::vector<std::string> fragments);
  public:
	NamespacedName();

	explicit NamespacedName(std::string_view name);

	[[nodiscard]] auto str() const -> std::string;

	[[nodiscard]] auto basename() const -> NamespacedName;

	[[nodiscard]] auto operator/(std::string_view part) const -> NamespacedName;

	[[nodiscard]] auto operator/(const NamespacedName& other) const -> NamespacedName;

	[[nodiscard]] auto getFragments() const -> const std::vector<std::string>&;
};

}  // namespace kodgen::view