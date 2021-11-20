//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <string>

#include "declBase.hpp"
#include "util/concepts.hpp"
#include "view/type/typeBase.hpp"

namespace kodgen::view {

template<util::Convertible<TypeBase> TypeT>
class VarDecl : public DeclBase {
	const TypeT type;

  public:
	VarDecl(const std::string_view& name, const TypeT type)
		: DeclBase(name)
		, type(type) {}

	[[nodiscard]] virtual TypeT getType() const { return type; };
};

}  // namespace kodgen::view