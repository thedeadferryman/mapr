//
// Created by kmeinkopf on 26.12.2021.
//

#include <sstream>

#include "namespacedName.hpp"

#include <boost/algorithm/string/join.hpp>

using boost::algorithm::join;
using kodgen::view::NamespacedName;

NamespacedName::NamespacedName() = default;

NamespacedName::NamespacedName(std::vector<std::string> fragments)
	: fragments(fragments) {}

NamespacedName::NamespacedName(std::string_view name)
	: fragments {std::string(name)} {}

auto NamespacedName::str() const -> std::string {
	return join(fragments, "::");
}

auto NamespacedName::basename() const -> NamespacedName {
	auto newFragments = std::vector<std::string>();

	if (fragments.empty()) {
		return NamespacedName();
	}

	std::copy(std::begin(fragments),
	          std::end(fragments) - 1,
	          std::back_inserter(newFragments));

	return NamespacedName(newFragments);
}

auto NamespacedName::operator/(std::string_view part) const -> NamespacedName {
	return *this / NamespacedName(part);
}

auto NamespacedName::operator/(const NamespacedName& other) const -> NamespacedName {
	auto newFragments = std::vector<std::string>();

	std::copy(fragments.begin(),
	          fragments.end(),
	          std::back_inserter(newFragments));

	std::copy(other.fragments.begin(),
	          other.fragments.end(),
	          std::back_inserter(newFragments));

	return NamespacedName(newFragments);
}
auto NamespacedName::getFragments() const -> const std::vector<std::string>& {
	return fragments;
}
