//
// Created by karl on 13.05.22.
//

#pragma once

#include "view/name/templates/templateArgumentBase.hpp"
namespace mapr::view {

class NullPtrTemplateArgument : public TemplateArgumentBase {
  public:
	auto str() const -> std::string override;
};

}  // namespace mapr::view
