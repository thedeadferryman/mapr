//
// Created by kmeinkopf on 20.11.2021.
//

#include <memory>
#include <sstream>

#include "declContext.hpp"

#include "view/exceptions/alreadyExistsException.hpp"

#include "util/stringBuilder.hpp"

using mapr::view::AlreadyExistsException;
using mapr::view::DeclBase;
using mapr::view::DeclContext;

void DeclContext::addDeclaration(const std::shared_ptr<const DeclBase>& decl) {
	const auto& declName = decl->getID();

	if (declarations.contains(declName)) {
		throw(AlreadyExistsException(declName, decl->getDeclType()));
	}

	declarations.insert(std::make_pair(declName, decl));
}

auto DeclContext::findDeclaration(std::string_view id) const
	-> std::shared_ptr<const view::DeclBase> {
	if (declarations.contains(std::string(id))) {
		return declarations.at(std::string(id));
	}

	return nullptr;
}

auto DeclContext::getDeclarations() const
	-> const std::map<std::string, std::shared_ptr<const view::DeclBase>>& {
	return declarations;
}
void DeclContext::replaceDeclaration(
	const std::shared_ptr<const DeclBase>& decl) {
	auto id = decl->getID();

	declarations[id] = decl;
}
