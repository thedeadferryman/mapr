//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <string>

#include <clang/AST/Type.h>

#include "view/declBase.hpp"
#include "view/types/typeBase.hpp"

namespace mapr::view {

class VarDecl : public DeclBase {
	const std::shared_ptr<TypeBase> type;
	std::shared_ptr<SourceLoc> location;

  public:
	VarDecl(std::string_view id,
	        std::shared_ptr<TypeBase> type,
	        std::shared_ptr<SourceLoc> location);

	[[nodiscard]] virtual auto getType() const
		-> const std::shared_ptr<TypeBase>&;

	friend auto operator==(const std::shared_ptr<const VarDecl>& first,
	                       const std::shared_ptr<const VarDecl>& second) {
		return first->getType() == second->getType();
	}

	auto getLocation() const -> std::shared_ptr<SourceLoc> override;
	auto getQualifiedName() const -> QualifiedName override;
};

}  // namespace mapr::view