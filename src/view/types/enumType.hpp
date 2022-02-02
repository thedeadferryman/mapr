//
// Created by kmeinkopf on 15.01.2022.
//

#pragma once

#include "view/namespacedName.hpp"
#include "view/types/typeBase.hpp"

namespace kodgen::view {

S_ENUM(EnumKind, Scoped, Leaking);

class EnumType : public TypeBase {
	NamespacedName name;
	bool scoped;

  public:
	EnumType(const clang::Qualifiers& quals, NamespacedName name, bool scoped);

	[[nodiscard]] auto getName() -> const NamespacedName&;

	[[nodiscard]] auto isScoped() -> bool;

	auto getPrettyName() const -> std::string override;
};

}  // namespace kodgen::view
