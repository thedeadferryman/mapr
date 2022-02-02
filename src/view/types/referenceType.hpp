//
// Created by kmeinkopf on 13.01.2022.
//

#pragma once

#include "view/types/typeBase.hpp"

namespace kodgen::view {

enum class ReferenceKind
{
	LVALUE,
	RVALUE,
	UNKNOWN
};

class ReferenceType : public TypeBase {
	ReferenceKind referenceKind;
	std::shared_ptr<TypeBase> referenced;

  public:
	ReferenceType(const clang::Qualifiers& quals,
	              std::shared_ptr<TypeBase> referenced,
	              ReferenceKind referenceKind);

	[[nodiscard]] auto getReferenceKind() const -> ReferenceKind;

	[[nodiscard]] auto dereference() const -> std::shared_ptr<TypeBase>;

	[[nodiscard]] auto getPrettyName() const -> std::string override;
};

}  // namespace kodgen::view
