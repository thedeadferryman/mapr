//
// Created by kmeinkopf on 02.02.2022.
//

#include "qualifiedNameTransformer.hpp"

#include <llvm/Support/raw_ostream.h>

#include "view/name/templates/typeTemplateArgument.hpp"

#include "transform/name/typeNameTransformer.hpp"

#include "util/collectionUtils.hpp"
#include "util/macro.hpp"
#include "util/matchType.hpp"
#include "util/stringBuilder.hpp"

using mapr::transform::QualifiedNameTransformer;
using mapr::util::CollectionUtils;
using mapr::util::MatchType;
using mapr::util::StringBuilder;

namespace algo = boost::algorithm;

mapr::transform::QualifiedNameTransformer::QualifiedNameTransformer(
	std::shared_ptr<config::PipelineContext> context)
	: context(std::move(context)) {}

auto QualifiedNameTransformer::getName(const view::QualifiedName& name,
                                       NameStyle style) const -> std::string {
	auto maybePrefix = context->readConfigVariable("namePrefix");

	if (!maybePrefix.has_value()) {
		UNREACHABLE();
	}

	auto str = StringBuilder((style == NameStyle::Prefixed)  //
	                             ? maybePrefix.value()
	                             : "");
	auto fragments = std::vector<std::string>();

	std::transform(std::begin(name.getFragments()),
	               std::end(name.getFragments()),
	               std::back_inserter(fragments),
	               [this](const view::NameFragment& fragment) {
					   return transformFragment(fragment);
				   });

	return str << algo::join_if(fragments, "_", [](std::string_view val) {
			   return !val.empty();
		   });
}
auto QualifiedNameTransformer::transformFragment(
	const view::NameFragment& fragment) const -> std::string {
	if (fragment.getTemplateArgs().empty()) {
		return fragment.str();
	}

	auto builder = StringBuilder(fragment.getValue());
	builder << "$";

	builder << CollectionUtils::mapJoin(
		fragment.getTemplateArgs(),
		"_",
		[this](
			const view::NameFragment::TemplateArgumentList::value_type& arg) {
			return transformTemplateArgument(arg);
		});

	return builder << "$";
}
auto mapr::transform::QualifiedNameTransformer::transformTemplateArgument(
	const std::shared_ptr<view::TemplateArgumentBase>& argument) const
	-> std::string {
	return MatchType::matchType<std::string>(
		argument,
		[this](const std::shared_ptr<view::TypeTemplateArgument>& arg) {
			return TypeNameTransformer(context).getOverloadSlug(arg->getType());
		},
		[](const std::shared_ptr<view::TemplateArgumentBase>& arg) {
			return arg->str();
		});
}
