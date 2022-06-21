//
// Created by kmeinkopf on 26.12.2021.
//

#include <sstream>
#include <utility>

#include "qualifiedName.hpp"

#include <boost/algorithm/string/join.hpp>

using boost::algorithm::join;
using mapr::view::QualifiedName;

QualifiedName::QualifiedName() = default;

QualifiedName::QualifiedName(std::vector<NameFragment> fragments)
	: fragments(std::move(fragments)) {}

QualifiedName::QualifiedName(NameFragment name)
	: fragments {std::move(name)} {}

mapr::view::QualifiedName::QualifiedName(
	std::string_view name,
	mapr::view::NameFragment::TemplateArgumentList templateArgs)
	: fragments {NameFragment(name, std::move(templateArgs))} {}

auto QualifiedName::str() const -> std::string {
	auto fragmentStrs = std::vector<std::string>();

	std::transform(std::begin(fragments),
	               std::end(fragments),
	               std::back_inserter(fragmentStrs),
	               [](const NameFragment& fragment) { return fragment.str(); });

	return join(fragmentStrs, "::");
}

auto QualifiedName::parent() const -> QualifiedName {
	auto newFragments = std::vector<NameFragment>();

	if (fragments.empty()) {
		return {};
	}

	std::copy(std::begin(fragments),
	          std::end(fragments) - 1,
	          std::back_inserter(newFragments));

	return QualifiedName(newFragments);
}

auto QualifiedName::operator/(std::string_view part) const -> QualifiedName {
	return *this / QualifiedName(part);
}

auto QualifiedName::operator/(const QualifiedName& other) const
	-> QualifiedName {
	auto newFragments = std::vector<NameFragment>();

	std::copy(fragments.begin(),  //
	          fragments.end(),
	          std::back_inserter(newFragments)  //
	);

	std::copy(other.fragments.begin(),
	          other.fragments.end(),
	          std::back_inserter(newFragments));

	return QualifiedName(newFragments);
}

auto QualifiedName::getFragments() const -> const std::vector<NameFragment>& {
	return fragments;
}

auto QualifiedName::basename() const -> QualifiedName {
	auto newFragments = std::vector<NameFragment> {*fragments.rbegin()};

	return QualifiedName(newFragments);
}
/**
 * The name is considered valid if there are no empty identifiers in it (except
 * the first one representing global namespace).
 *
 * @return bool Is the name valid
 */
auto mapr::view::QualifiedName::isValid() const -> bool {
	for (auto it = std::begin(fragments) + 1; it != fragments.end(); it++) {
		if (it->getValue().empty()) {
			return false;
		}
	}

	return true;
}
