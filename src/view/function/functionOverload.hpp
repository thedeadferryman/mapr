//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <optional>
#include <variant>
#include <vector>

#include <clang/AST/Type.h>

#include "view/declBase.hpp"
#include "view/loc/singleSourceLoc.hpp"
#include "view/varDecl.hpp"

namespace mapr::view {

class FunctionOverload {
	std::shared_ptr<TypeBase> returnType;
	std::vector<std::shared_ptr<const VarDecl>> arguments;
	std::shared_ptr<SingleSourceLoc> location;

  public:
	FunctionOverload(std::shared_ptr<TypeBase> returnType,
	                 std::vector<std::shared_ptr<const VarDecl>> arguments,
	                 std::shared_ptr<SingleSourceLoc> location);

	[[nodiscard]] auto getReturnType() const
		-> const std::shared_ptr<TypeBase>&;

	[[nodiscard]] auto getArguments() const
		-> const std::vector<std::shared_ptr<const VarDecl>>&;

	[[nodiscard]] auto getLocation() const
		-> const std::shared_ptr<SingleSourceLoc>&;

	[[nodiscard]] auto countArguments() const -> std::size_t;

	[[nodiscard]] auto bindArgument() const -> std::optional<FunctionOverload>;

	[[nodiscard]] auto appendArgument(std::shared_ptr<const VarDecl> argument) const
		-> FunctionOverload;

	friend auto operator==(const FunctionOverload& first,
	                       const FunctionOverload& second) -> bool {
		return (first.arguments.size() == second.arguments.size())
			&& std::equal(std::begin(first.arguments),
		                  std::end(first.arguments),
		                  std::begin(second.arguments));
	}
};

}  // namespace mapr::view