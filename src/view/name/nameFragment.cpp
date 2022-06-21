//
// Created by karl on 12.05.22.
//

#include "nameFragment.hpp"

#include <boost/algorithm/string/join.hpp>

#include "util/collectionUtils.hpp"
#include "util/stringBuilder.hpp"

using boost::algorithm::join;
using mapr::util::CollectionUtils;
using mapr::util::StringBuilder;
using mapr::view::NameFragment;

NameFragment::NameFragment(std::string_view value,
                           TemplateArgumentList templateArgs)
	: value(value)
	, templateArgs(std::move(templateArgs)) {}

auto NameFragment::getValue() const -> const std::string& {
	return value;
}

auto NameFragment::getTemplateArgs() const
	-> const NameFragment::TemplateArgumentList& {
	return templateArgs;
}
auto NameFragment::str() const -> std::string {
	if (templateArgs.empty()) {
		return value;
	}

	auto builder = StringBuilder();

	builder << value << "<";

	return builder << CollectionUtils::mapJoin(
			   templateArgs,
			   ", ",
			   [](const TemplateArgumentList::value_type& arg) {
				   return arg->str();
			   }) << ">";
}
