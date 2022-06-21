//
// Created by karl on 12.05.22.
//

#pragma once

#include <llvm/ADT/APSInt.h>

#include "view/name/templates/templateArgumentBase.hpp"

namespace mapr::view {

class IntegralTemplateArgument : public TemplateArgumentBase {
	llvm::APSInt value;

  public:
	explicit IntegralTemplateArgument(llvm::APSInt value);

	[[nodiscard]] auto str() const -> std::string override;
};

}  // namespace mapr::view