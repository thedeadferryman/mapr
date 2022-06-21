//
// Created by kmeinkopf on 13.01.2022.
//

#pragma once

#include <clang/AST/Type.h>

#include "util/macro.hpp"

namespace mapr::view {

S_ENUM(TypeKind, Record, Builtin, Pointer, Reference, Alias, Enum);

class TypeBase {
	clang::Qualifiers qualifiers;
	TypeKind kind;

  public:
	TypeBase(TypeKind kind, const clang::Qualifiers& qualifiers);

	[[nodiscard]] auto getQualifiers() const -> const clang::Qualifiers&;
	[[nodiscard]] auto getKind() const -> TypeKind;

	[[nodiscard]] virtual auto getPrettyName() const -> std::string = 0;

	[[nodiscard]] virtual auto isVoid() const -> bool;

	TypeBase(const TypeBase&) = delete;
	TypeBase(TypeBase&&) = delete;
	auto operator=(const TypeBase&) -> TypeBase& = delete;
	auto operator=(TypeBase&&) -> TypeBase& = delete;
	virtual ~TypeBase() = default;

	friend auto operator==(const std::shared_ptr<TypeBase>& first,
	                       const std::shared_ptr<TypeBase>& second) -> bool {
		return (first->getQualifiers() == second->getQualifiers())
			&& (first->getPrettyName() == second->getPrettyName());
	}

	friend auto operator<<(std::ostream& stream,
	                       const std::shared_ptr<TypeBase>& type)
		-> decltype(stream) {
		return stream << type->getPrettyName();
	}
};
}  // namespace mapr::view
