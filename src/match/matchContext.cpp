//
// Created by kmeinkopf on 20.11.2021.
//

#include <memory>
#include "matchContext.hpp"

using kodgen::match::MatchContext;
using kodgen::view::DeclBase;

void MatchContext::addDeclaration(std::shared_ptr<DeclBase> decl) {
	declarations.push_back(decl);
}

auto MatchContext::findDeclaration(const std::string_view& type)
	-> std::optional<DeclBase> {
	for (auto decl : declarations) {
		if (decl->getName() == type) {
			return *decl;
		}
	}

	return std::nullopt;
}

const std::vector<std::shared_ptr<DeclBase>>&
kodgen::match::MatchContext::getDeclarations() const {
	return declarations;
}
