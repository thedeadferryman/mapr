//
// Created by karl on 13.05.22.
//

#include <string>

#include "typeTemplateArgument.hpp"

using mapr::view::TypeTemplateArgument;

TypeTemplateArgument::TypeTemplateArgument(std::shared_ptr<TypeBase> type)
	: type(std::move(type)) {}

auto TypeTemplateArgument::str() const -> std::string {
	return type->getPrettyName();
}
auto TypeTemplateArgument::getType() const -> const std::shared_ptr<TypeBase>& {
	return type;
}
