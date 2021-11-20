//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <vector>

#include "view/declBase.hpp"
#include "view/type/typeBase.hpp"
#include "view/varDecl.hpp"

namespace kodgen::view {

template<util::Convertible<TypeBase> TypeT>
class FunctionBase : public DeclBase {
	TypeT returnType;
	std::vector<VarDecl<TypeT>> arguments;

  public:
	FunctionBase(const std::string_view& name,
	             TypeT returnType,
	             const std::vector<VarDecl<TypeT>>& arguments)
		: DeclBase(name)
		, returnType(returnType)
		, arguments(arguments) {}

	TypeT getReturnType() const { return returnType; }

	const std::vector<VarDecl<TypeT>>& getArguments() const {
		return arguments;
	}
};

}  // namespace kodgen::view