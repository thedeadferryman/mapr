//
// Created by kmeinkopf on 26.01.2022.
//

#pragma once

#include "view/declBase.hpp"
#include "view/types/typeBase.hpp"

namespace mapr::transform {

class TypeDecl : public view::DeclBase {
	std::shared_ptr<view::TypeBase> type;

  public:
	explicit TypeDecl(std::shared_ptr<view::TypeBase> type);

	[[nodiscard]] auto getType() const -> std::shared_ptr<view::TypeBase>;

	[[nodiscard]] auto getLocation() const
		-> std::shared_ptr<view::SourceLoc> override;

	[[nodiscard]] auto getQualifiedName() const -> view::QualifiedName override;
};

}  // namespace mapr::transform