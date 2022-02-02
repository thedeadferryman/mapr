//
// Created by kmeinkopf on 07.01.2022.
//

#pragma once

#include <vector>

#include "view/declBase.hpp"
#include "view/types/enumType.hpp"

#include "enumMemberDecl.hpp"

namespace kodgen::view {

class EnumDecl : public DeclBase {
  public:
	using Member = EnumMemberDecl;

  private:
	std::shared_ptr<EnumType> type;
	std::vector<std::shared_ptr<Member>> members;

  public:
	EnumDecl(std::shared_ptr<EnumType> type,
	         std::vector<std::shared_ptr<Member>> members = {});

	[[nodiscard]] auto getType() const -> const std::shared_ptr<EnumType>&;

	[[nodiscard]] auto getMembers() const
		-> const std::vector<std::shared_ptr<Member>>&;

	void addMember(const std::shared_ptr<Member>& member);
};

}  // namespace kodgen::view
