//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <string>

#include <clang/AST/Type.h>

#include "view/declBase.hpp"
#include "view/types/typeBase.hpp"

namespace kodgen::view {

class VarDecl : public DeclBase {
	const std::shared_ptr<TypeBase> type;

  public:
	VarDecl(std::string_view id, std::shared_ptr<TypeBase> type);

	[[nodiscard]] virtual auto getType() const
		-> const std::shared_ptr<TypeBase>&;
};

}  // namespace kodgen::view