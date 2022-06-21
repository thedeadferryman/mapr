//
// Created by karl on 14.05.22.
//

#include "functionHelpers.hpp"

#include <fmt/format.h>

using mapr::match::FunctionHelpers;

auto FunctionHelpers::getParmName(const clang::ParmVarDecl* parmDecl)
	-> std::string {
	auto ownName = parmDecl->getName().str();
	auto fallbackName = fmt::format("_{:x}", parmDecl->getID());

	return ownName.empty() ? fallbackName : ownName;
}
