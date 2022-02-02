//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <vector>

#include <clang/AST/Type.h>
#include <variant>
#include <optional>

#include "view/declBase.hpp"
#include "view/varDecl.hpp"

namespace kodgen::view {

class FunctionOverload {
	std::shared_ptr<TypeBase> returnType;
	std::vector<std::shared_ptr<VarDecl>> arguments;

  public:
	FunctionOverload(std::shared_ptr<TypeBase> returnType,
	                 std::vector<std::shared_ptr<VarDecl>> arguments);

	[[nodiscard]] auto getReturnType() const
		-> const std::shared_ptr<TypeBase>&;

	[[nodiscard]] auto getArguments() const
		-> const std::vector<std::shared_ptr<VarDecl>>&;

	[[nodiscard]] auto countArguments() const -> std::size_t;

	[[nodiscard]] auto bindArgument() const -> std::optional<FunctionOverload>;

	[[nodiscard]] auto appendArgument(std::shared_ptr<VarDecl> argument) const -> FunctionOverload;
};

}  // namespace kodgen::view