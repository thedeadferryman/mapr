//
// Created by karl on 13.05.22.
//

#pragma once

#include <memory>

#include "view/types/typeBase.hpp"

#include "view/name/templates/templateArgumentBase.hpp"

namespace mapr::view {

class TypeTemplateArgument : public TemplateArgumentBase {
	std::shared_ptr<TypeBase> type;

  public:
	explicit TypeTemplateArgument(std::shared_ptr<TypeBase> type);

	[[nodiscard]] auto str() const -> std::string override;

	[[nodiscard]] auto getType() const -> const std::shared_ptr<TypeBase>&;
};

}  // namespace mapr::view
