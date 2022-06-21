//
// Created by kmeinkopf on 07.01.2022.
//

#include "enumDecl.hpp"

#include "view/name/qualifiedName.hpp"

using mapr::view::EnumDecl;

EnumDecl::EnumDecl(const std::shared_ptr<EnumType>& type,
                   std::shared_ptr<SourceLoc> location,
                   std::vector<std::shared_ptr<Member>> members)
	: DeclBase(type->getName().str(), DeclType::Enum)
	, type(type)
	, members(std::move(members))
	, location(std::move(location)) {}

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
auto EnumDecl::getLocation() const -> std::shared_ptr<SourceLoc> {
	return location;
}

auto EnumDecl::getQualifiedName() const -> QualifiedName {
	return type->getName();
}
