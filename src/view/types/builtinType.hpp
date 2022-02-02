//
// Created by kmeinkopf on 13.01.2022.
//

#pragma once

#include "view/types/typeBase.hpp"

namespace kodgen::view {

class BuiltinType : public TypeBase {
  public:
	using Variant = clang::BuiltinType::Kind;

  private:
	Variant variant;
	std::string displayName;

  public:
	BuiltinType(const clang::Qualifiers& qualType,
	            std::string_view displayName,
	            Variant kind);

	[[nodiscard]] auto getVariant() const -> Variant;

	[[nodiscard]] auto getDisplayName() const -> std::string;

	[[nodiscard]] auto getPrettyName() const -> std::string override;
};

}  // namespace kodgen::view
