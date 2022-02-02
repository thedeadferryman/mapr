//
// Created by kmeinkopf on 19.01.2022.
//

#include "functionNameTransformer.hpp"

#include <llvm/Support/raw_ostream.h>

#include "transform/name/namespacedNameTransformer.hpp"
#include "transform/name/typeNameTransformer.hpp"

#include "util/stringBuilder.hpp"

using kodgen::transform::FunctionNameTransformer;

auto FunctionNameTransformer::getOverloadName(
	const view::NamespacedName& functionName,
	const view::FunctionOverload& overload,
	SlugType slugType) -> std::string {
	if (slugType == SlugType::NoSlug) {
		return NamespacedNameTransformer::getName(functionName);
	}

	return NamespacedNameTransformer::getName(functionName)
		+ functionParamSuffix(overload, slugType);
}

auto FunctionNameTransformer::functionParamSuffix(
	const view::FunctionOverload& overload, SlugType slugType) -> std::string {
	auto typeSlugs = std::vector<std::string>();

	if (slugType == SlugType::CountSlug) {
		return "_" + std::to_string(overload.countArguments());
	}

	std::transform(
		std::begin(overload.getArguments()),
		std::end(overload.getArguments()),
		std::back_inserter(typeSlugs),
		[](const std::shared_ptr<view::VarDecl>& decl) {
			return TypeNameTransformer::getOverloadSlug(decl->getType());
		});

	return "_" + boost::algorithm::join(typeSlugs, "$");
}

#pragma clang diagnostic pop
