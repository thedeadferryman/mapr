//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include "view/types/builtinType.hpp"
#include "view/types/typeBase.hpp"

#include "util/exceptions/baseException.hpp"

namespace kodgen::transform {

class UnsupportedTypeException : public util::BaseException {
  public:
	UnsupportedTypeException(view::BuiltinType::Variant variant,
	                         std::string displayName);

	UnsupportedTypeException(view::TypeKind kind, std::string_view typeName);
};

}  // namespace kodgen::getName
