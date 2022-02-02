//
// Created by kmeinkopf on 06.01.2022.
//

#include <span>
#include <utility>

#include "functionOverload.hpp"

using kodgen::view::FunctionOverload;

FunctionOverload::FunctionOverload(
	std::shared_ptr<TypeBase> returnType,
	std::vector<std::shared_ptr<VarDecl>> arguments)
	: returnType(std::move(returnType))
	, arguments(std::move(arguments)) {}

auto FunctionOverload::getReturnType() const
	-> const std::shared_ptr<TypeBase>& {
	return returnType;
}

auto FunctionOverload::getArguments() const
	-> const std::vector<std::shared_ptr<VarDecl>>& {
	return arguments;
}

auto FunctionOverload::countArguments() const -> std::size_t {
	return arguments.size();
}

auto FunctionOverload::bindArgument() const -> std::optional<FunctionOverload> {
	if (arguments.empty())
		return std::nullopt;

	if (arguments.size() <= 1) {
		return FunctionOverload(returnType, {});
	}

	auto newArgs = decltype(arguments)();

	std::copy(std::begin(arguments) + 1,
	          std::end(arguments),
	          std::back_inserter(newArgs));

	return FunctionOverload(returnType, std::move(newArgs));
}

auto FunctionOverload::appendArgument(std::shared_ptr<VarDecl> argument) const
	-> FunctionOverload {
	auto newArgs = decltype(arguments)();

	std::copy(std::begin(arguments),
	          std::end(arguments),
	          std::back_inserter(newArgs));

	newArgs.push_back(std::move(argument));

	return FunctionOverload(returnType, std::move(newArgs));
}
