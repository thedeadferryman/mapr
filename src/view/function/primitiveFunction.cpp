//
// Created by kmeinkopf on 20.11.2021.
//

#include "primitiveFunction.hpp"

using kodgen::view::PrimitiveFunction;
using kodgen::view::PrimitiveType;
using kodgen::view::VarDecl;

PrimitiveFunction::PrimitiveFunction(
	const std::string_view& name,
	const PrimitiveType& returnType,
	const std::vector<VarDecl<PrimitiveType>>& arguments)
	: FunctionBase(name, returnType, arguments) {}
