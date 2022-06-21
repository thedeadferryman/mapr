//
// Created by kmeinkopf on 15.01.2022.
//

#include "enumType.hpp"

using mapr::view::EnumType;

EnumType::EnumType(const clang::Qualifiers& quals,
                   QualifiedName name,
                   bool scoped)
	: TypeBase(TypeKind::Enum, quals)
	, name(std::move(name))
	, scoped(scoped) {}

auto EnumType::getName() -> const QualifiedName& {
	return name;
}

void EnumType::setName(QualifiedName newName) {
	name = std::move(newName);
}

auto EnumType::isScoped() -> bool {
	return scoped;
}
std::string mapr::view::EnumType::getPrettyName() const {
	return name.str();
}
