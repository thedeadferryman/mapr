//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include "view/declBase.hpp"
#include "view/loc/sourceLoc.hpp"

namespace mapr::transform {

class AuxDecl : public view::DeclBase {
  public:
	explicit AuxDecl(std::string_view declId);

	auto getLocation() const -> std::shared_ptr<view::SourceLoc> override;
	auto getQualifiedName() const -> view::QualifiedName override;
};

}  // namespace mapr::transform