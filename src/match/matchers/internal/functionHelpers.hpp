//
// Created by karl on 14.05.22.
//

#pragma once

#include <clang/AST/Decl.h>
namespace mapr {
namespace match {

class FunctionHelpers {
  public:
	[[nodiscard]] static auto getParmName(const clang::ParmVarDecl* parmDecl)
		-> std::string;
};

}  // namespace match
}  // namespace mapr
