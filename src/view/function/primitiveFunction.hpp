//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include "functionBase.hpp"
#include "view/type/primitiveType.hpp"

namespace kodgen::view {

class PrimitiveFunction : public FunctionBase<PrimitiveType> {
  public:
	PrimitiveFunction(const std::string_view& name,
	                  const PrimitiveType& returnType,
	                  const std::vector<VarDecl<PrimitiveType>>& arguments);
};

}  // namespace kodgen::view