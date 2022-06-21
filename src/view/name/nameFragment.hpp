//
// Created by karl on 12.05.22.
//

#pragma once

#include <memory>
#include <vector>

#include "view/name/templates/templateArgumentBase.hpp"

namespace mapr::view {

class NameFragment {
  public:
	using TemplateArgumentList =
		std::vector<std::shared_ptr<TemplateArgumentBase>>;

  private:
	std::string value;
	TemplateArgumentList templateArgs;

  public:
	explicit NameFragment(std::string_view value,
	                      TemplateArgumentList templateArgs = {});

	[[nodiscard]] auto getValue() const -> const std::string&;
	[[nodiscard]] auto getTemplateArgs() const -> const TemplateArgumentList&;

	[[nodiscard]] auto str() const -> std::string;
};

}  // namespace mapr::view
