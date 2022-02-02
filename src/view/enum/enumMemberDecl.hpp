//
// Created by kmeinkopf on 07.01.2022.
//

#pragma once

#include <memory>
#include <optional>

#include <clang/AST/Type.h>

#include "view/declBase.hpp"

namespace kodgen::view {

class EnumDecl;

class EnumMemberDecl : public DeclBase {
	std::weak_ptr<EnumDecl> owner;

  public:
	EnumMemberDecl(const std::string& id,
	               std::shared_ptr<EnumDecl> owner);

	[[nodiscard]] auto getOwner() -> std::shared_ptr<EnumDecl>;

	[[nodiscard]] auto getName() -> std::string;
};

}  // namespace kodgen::view
