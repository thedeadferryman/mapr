//
// Created by kmeinkopf on 07.01.2022.
//

#include "enumMemberDecl.hpp"

#include "enumDecl.hpp"

using kodgen::view::EnumMemberDecl;

EnumMemberDecl::EnumMemberDecl(const std::string& id,
                               std::shared_ptr<EnumDecl> owner)
	: DeclBase(id, DeclType::EnumMember)
	, owner(std::move(owner)) {}

auto EnumMemberDecl::getOwner() -> std::shared_ptr<EnumDecl> {
	return owner.lock();
}

auto EnumMemberDecl::getName() -> std::string {
	return getID();
}