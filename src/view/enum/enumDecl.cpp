//
// Created by kmeinkopf on 07.01.2022.
//

#include "enumDecl.hpp"

using kodgen::view::EnumDecl;

EnumDecl::EnumDecl(std::shared_ptr<EnumType> type,
                   std::vector<std::shared_ptr<Member>> members)
	: DeclBase(type->getName().str(), DeclType::Enum)
	, type(type)
	, members(std::move(members)) {}

auto EnumDecl::getType() const -> const std::shared_ptr<EnumType>& {
	return type;
}

auto EnumDecl::getMembers() const
	-> const std::vector<std::shared_ptr<EnumDecl::Member>>& {
	return members;
}

void EnumDecl::addMember(const std::shared_ptr<Member>& member) {
	members.push_back(member);
}
