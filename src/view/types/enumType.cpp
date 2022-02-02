//
// Created by kmeinkopf on 15.01.2022.
//

#include "enumType.hpp"

using kodgen::view::EnumType;

EnumType::EnumType(const clang::Qualifiers& quals,
                   NamespacedName name,
                   bool scoped)
	: TypeBase(TypeKind::Enum, quals)
	, name(std::move(name))
	, scoped(scoped) {}

auto EnumType::getName() -> const NamespacedName& {
	return name;
}

auto EnumType::isScoped() -> bool {
	return scoped;
}
std::string kodgen::view::EnumType::getPrettyName() const {
	return name.str();
}
