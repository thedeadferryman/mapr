//
// Created by kmeinkopf on 07.01.2022.
//

#pragma once

#include <memory>
#include <optional>

#include <clang/AST/Type.h>

#include "view/declBase.hpp"
#include "view/name/qualifiedName.hpp"

namespace mapr::view {

class EnumDecl;

class EnumMemberDecl : public DeclBase {
	std::weak_ptr<const EnumDecl> owner;
	std::string value;
	std::shared_ptr<SourceLoc> location;
	QualifiedName name;

  public:
	EnumMemberDecl(const QualifiedName& name,
	               const std::shared_ptr<const EnumDecl>& owner,
	               std::string_view value,
	               std::shared_ptr<SourceLoc> location);

	[[nodiscard]] auto getOwner() const -> std::shared_ptr<const EnumDecl>;

	[[nodiscard]] auto getValue() const -> std::string;

	[[nodiscard]] auto getName() const -> std::string;

	auto getLocation() const -> std::shared_ptr<SourceLoc> override;

	auto getQualifiedName() const -> QualifiedName override;
};

}  // namespace mapr::view
