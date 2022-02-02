//
// Created by kmeinkopf on 13.01.2022.
//

#pragma once

#include <clang/AST/Type.h>

#include "util/macro.hpp"

namespace kodgen::view {

S_ENUM(TypeKind, Class, Struct, Builtin, Pointer, Reference, Alias, Enum);

class TypeBase {
	clang::Qualifiers qualifiers;
	TypeKind kind;

  public:
	TypeBase(TypeKind kind, const clang::Qualifiers& qualifiers);

	[[nodiscard]] auto getQualifiers() const -> const clang::Qualifiers&;
	[[nodiscard]] auto getKind() const -> TypeKind;

	[[nodiscard]] virtual auto getPrettyName() const -> std::string = 0;

	TypeBase(const TypeBase&) = delete;
	TypeBase(TypeBase&&) = delete;
	auto operator=(const TypeBase&) -> TypeBase& = delete;
	auto operator=(TypeBase&&) -> TypeBase& = delete;
	virtual ~TypeBase() = default;

	friend auto operator<<(std::ostream& os, std::shared_ptr<TypeBase> type)
		-> decltype(os) {
		return os << type->getPrettyName();
	};
};

}  // namespace kodgen::view
