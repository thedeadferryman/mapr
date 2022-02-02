//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include "view/declBase.hpp"

namespace kodgen::transform {

class AuxDecl : public view::DeclBase {
  public:
	explicit AuxDecl(std::string_view declId);
};

}  // namespace kodgen::getName