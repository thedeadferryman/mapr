//
// Created by kmeinkopf on 07.01.2022.
//

#include <sstream>

#include "aliasType.hpp"

using kodgen::view::AliasType;

AliasType::AliasType(const clang::Qualifiers& quals,
                     std::string_view name,
                     std::shared_ptr<TypeBase> source)
	: TypeBase(TypeKind::Alias, quals)
	, source(std::move(source))
	, name(name) {}

auto kodgen::view::AliasType::getSource() const
	-> const std::shared_ptr<TypeBase>& {
	return source;
}

auto kodgen::view::AliasType::getQualifiedName() const -> const std::string& {
	return name;
}

auto kodgen::view::AliasType::getPrettyName() const -> std::string {
	std::stringstream builder(name);

	if (getQualifiers().hasConst()) {
		builder << "const ";
	}

	builder << " /* " << source->getPrettyName() << " */";

	return builder.str();
}
