//
// Created by kmeinkopf on 15.01.2022.
//

#pragma once

#include "view/name/qualifiedName.hpp"
#include "view/types/typeBase.hpp"

namespace mapr::view {

S_ENUM(EnumKind, Scoped, Leaking);

class EnumType : public TypeBase {
	QualifiedName name;
	bool scoped;

  public:
	EnumType(const clang::Qualifiers& quals, QualifiedName name, bool scoped);

	[[nodiscard]] auto getName() -> const QualifiedName&;

	void setName(QualifiedName newName);

	[[nodiscard]] auto isScoped() -> bool;

	auto getPrettyName() const -> std::string override;
};

}  // namespace mapr::view
