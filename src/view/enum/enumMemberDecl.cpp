//
// Created by kmeinkopf on 07.01.2022.
//

#include "enumMemberDecl.hpp"

#include "view/enum/enumDecl.hpp"

using mapr::view::EnumMemberDecl;

EnumMemberDecl::EnumMemberDecl(const QualifiedName& name,
                               const std::shared_ptr<const EnumDecl>& owner,
                               std::string_view value,
                               std::shared_ptr<SourceLoc> location)
	: DeclBase(name.str(), DeclType::EnumMember)
	, owner(owner)
	, value(value)
	, location(std::move(location))
	, name(name) {}

auto EnumMemberDecl::getOwner() const -> std::shared_ptr<const EnumDecl> {
	return owner.lock();
}

auto EnumMemberDecl::getValue() const -> std::string {
	return value;
}

auto EnumMemberDecl::getName() const -> std::string {
	return name.basename().str();
}

auto EnumMemberDecl::getLocation() const -> std::shared_ptr<SourceLoc> {
	return location;
}
auto EnumMemberDecl::getQualifiedName() const -> QualifiedName {
	return name;
}
