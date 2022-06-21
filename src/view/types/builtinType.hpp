//
// Created by kmeinkopf on 13.01.2022.
//

#pragma once

#include "view/types/typeBase.hpp"

namespace mapr::view {

class BuiltinType : public TypeBase {
  public:
	using Variant = clang::BuiltinType::Kind;

	[[nodiscard]] static auto makeVoid() -> std::shared_ptr<BuiltinType>;

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

	[[nodiscard]] auto isVoid() const -> bool override;
};

}  // namespace mapr::view
