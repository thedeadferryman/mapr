//
// Created by kmeinkopf on 19.01.2022.
//

#include "unsupportedTypeException.hpp"

#include "util/stringBuilder.hpp"

using mapr::transform::UnsupportedTypeException;

UnsupportedTypeException::UnsupportedTypeException(
	view::BuiltinType::Variant variant, std::string displayName)
	: BaseException(std::string(util::StringBuilder("Unsupported builtin type ")
                                << displayName << " (aux ID: "
                                << static_cast<std::int32_t>(variant) << ")")) {
}

UnsupportedTypeException::UnsupportedTypeException(  //
	mapr::view::TypeKind kind,
	std::string_view typeName)
	: BaseException(std::string(  //
		util::StringBuilder("Unsupported type ")
		<< view::stringOfTypeKind(kind) << ": " << typeName)) {}
