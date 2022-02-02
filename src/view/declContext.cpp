//
// Created by kmeinkopf on 20.11.2021.
//

#include <memory>
#include <sstream>

#include "declContext.hpp"

#include "view/exceptions/alreadyExistsException.hpp"

#include "util/stringBuilder.hpp"

using kodgen::view::AlreadyExistsException;
using kodgen::view::DeclBase;
using kodgen::view::DeclContext;

void DeclContext::addDeclaration(const std::shared_ptr<DeclBase>& decl) {
	const auto& declName = decl->getID();

	if (declarations.contains(declName)) {
		throw(AlreadyExistsException(declName, decl->getDeclType()));
	}

	declarations.insert(std::make_pair(declName, decl));
}

auto DeclContext::findDeclaration(std::string_view id) const
	-> std::shared_ptr<view::DeclBase> {
	for (auto [name, decl] : declarations) {
		if (name == id) {
			return decl;
		}
	}

	return nullptr;
}

auto DeclContext::getDeclarations() const
	-> const std::map<std::string, std::shared_ptr<view::DeclBase>>& {
	return declarations;
}
