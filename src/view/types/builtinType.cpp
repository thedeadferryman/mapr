//
// Created by kmeinkopf on 13.01.2022.
//

#include <sstream>

#include "builtinType.hpp"

using mapr::view::BuiltinType;

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

auto BuiltinType::isVoid() const -> bool {
	return variant == Variant::Void;
}

auto BuiltinType::makeVoid() -> std::shared_ptr<BuiltinType> {
	return std::make_shared<view::BuiltinType>(  //
		clang::Qualifiers(),
		std::string("void"),
		view::BuiltinType::Variant::Void  //
	);
}
