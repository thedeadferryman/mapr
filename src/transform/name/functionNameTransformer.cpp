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
	switch (slugType) {
		case SlugType::NoSlug:
			return getOverloadName(functionName);
		case SlugType::CountSlug:
			return getOverloadName(functionName, overload.countArguments());
		case SlugType::FullSlug:
			return getOverloadName(functionName, overload);
		default:
			UNREACHABLE();
	}
}

auto FunctionNameTransformer::getOverloadName(
	const view::NamespacedName& functionName,
	const view::FunctionOverload& overload) -> std::string {
	return getOverloadName(functionName) + getOverloadTypedSuffix(overload);
}

auto FunctionNameTransformer::getOverloadName(
	const view::NamespacedName& functionName) -> std::string {
	return NamespacedNameTransformer::getName(functionName);
}

auto FunctionNameTransformer::getOverloadName(
	const view::NamespacedName& functionName, std::size_t argCount)
	-> std::string {
	return getOverloadName(functionName) + getOverloadCountSuffix(argCount);
}

auto FunctionNameTransformer::getOverloadTypedSuffix(
	const view::FunctionOverload& overload) -> std::string {
	auto typeSlugs = std::vector<std::string>();

	std::transform(
		std::begin(overload.getArguments()),
		std::end(overload.getArguments()),
		std::back_inserter(typeSlugs),
		[](const std::shared_ptr<view::VarDecl>& decl) {
			return TypeNameTransformer::getOverloadSlug(decl->getType());
		});

	return "_" + boost::algorithm::join(typeSlugs, "$");
}

auto FunctionNameTransformer::getOverloadCountSuffix(std::size_t argCount)
	-> std::string {
	if (argCount == 0) {
		return "$1$$01";
	} else if (argCount == 1) {
		return "$1$$00";
	} else {
		return util::StringBuilder() << "$" << argCount << "$$11";
	}
}

#pragma clang diagnostic pop
