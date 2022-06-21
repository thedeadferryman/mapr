//
// Created by karl on 12.05.22.
//

#pragma once

#include "util/macro.hpp"

namespace mapr::view {

class TemplateArgumentBase {
  public:
	TemplateArgumentBase() = default;

	[[nodiscard]] virtual auto str() const -> std::string = 0;

	TemplateArgumentBase(const TemplateArgumentBase&) = delete;
	TemplateArgumentBase(TemplateArgumentBase&&) = delete;
	auto operator=(const TemplateArgumentBase&)
		-> TemplateArgumentBase& = delete;
	auto operator=(TemplateArgumentBase&&) -> TemplateArgumentBase& = delete;
	virtual ~TemplateArgumentBase() = default;
};

}  // namespace mapr::view
