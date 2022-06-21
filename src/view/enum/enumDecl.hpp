//
// Created by kmeinkopf on 07.01.2022.
//

#pragma once

#include <vector>

#include "view/declBase.hpp"
#include "view/enum/enumMemberDecl.hpp"
#include "view/types/enumType.hpp"

namespace mapr::view {

class EnumDecl : public DeclBase {
  public:
	using Member = EnumMemberDecl;

  private:
	std::shared_ptr<EnumType> type;
	std::vector<std::shared_ptr<Member>> members;
	std::shared_ptr<SourceLoc> location;

  public:
	explicit EnumDecl(const std::shared_ptr<EnumType>& type,
	                  std::shared_ptr<SourceLoc> location,
	                  std::vector<std::shared_ptr<Member>> members = {});

	[[nodiscard]] auto getType() const -> const std::shared_ptr<EnumType>&;

	[[nodiscard]] auto getMembers() const
		-> const std::vector<std::shared_ptr<Member>>&;

	auto getLocation() const -> std::shared_ptr<SourceLoc> override;

	auto getQualifiedName() const -> QualifiedName override;

	void addMember(const std::shared_ptr<Member>& member);
};

}  // namespace mapr::view
