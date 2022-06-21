//
// Created by kmeinkopf on 07.01.2022.
//

#include <sstream>

#include "aliasType.hpp"

#include "util/stringBuilder.hpp"

using mapr::view::AliasType;

AliasType::AliasType(const clang::Qualifiers& quals,
                     QualifiedName name,
                     std::shared_ptr<TypeBase> source)
	: TypeBase(TypeKind::Alias, quals)
	, source(std::move(source))
	, name(name) {}

auto AliasType::getSource() const
	-> const std::shared_ptr<TypeBase>& {
	return source;
}

auto AliasType::getName() const -> const QualifiedName& {
	return name;
}

auto AliasType::getPrettyName() const -> std::string {
	auto builder = util::StringBuilder(name.str());

	if (getQualifiers().hasConst()) {
		builder << "const ";
	}

	return builder;
}
