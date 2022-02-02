//
// Created by kmeinkopf on 26.01.2022.
//

#pragma once

#include "view/declBase.hpp"
#include "view/types/typeBase.hpp"

namespace kodgen::transform {

class TypeDecl : public view::DeclBase {
	std::shared_ptr<view::TypeBase> type;

  public:
	explicit TypeDecl(std::shared_ptr<view::TypeBase> type);

	[[nodiscard]] auto getType() -> std::shared_ptr<view::TypeBase>;
};

}  // namespace kodgen::getName