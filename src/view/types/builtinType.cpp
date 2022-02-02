//
// Created by kmeinkopf on 13.01.2022.
//

#include <sstream>

#include "builtinType.hpp"

using kodgen::view::BuiltinType;

BuiltinType::BuiltinType(const clang::Qualifiers& qualifiers,
                         std::string_view displayName,
                         Variant kind)
	: TypeBase(TypeKind::Builtin, qualifiers)
	, variant(kind)
	, displayName(displayName) {}

auto BuiltinType::getVariant() const -> Variant {
	return variant;
}
auto BuiltinType::getPrettyName() const -> std::string {
	std::stringstream builder;

	if (getQualifiers().hasConst()) {
		builder << "const ";
	}

	builder << displayName;

	return builder.str();
}
auto BuiltinType::getDisplayName() const -> std::string {
	return displayName;
}
