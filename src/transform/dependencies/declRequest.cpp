//
// Created by kmeinkopf on 24.01.2022.
//

#include "declRequest.hpp"

using mapr::transform::DeclRequest;

DeclRequest::DeclRequest(std::string_view declId)
	: DependencyRequest(DependencyKind::Decl)
	, declId(declId) {}

auto DeclRequest::getDeclarationId() const -> const std::string& {
	return declId;
}
