//
// Created by karl on 31.03.22.
//

#include <utility>

#include "recordCtorDecl.hpp"

using mapr::view::RecordCtorDecl;

RecordCtorDecl::RecordCtorDecl(std::string_view id,
                               QualifiedName qualifiedName,
                               const FunctionOverload& func)
	: FunctionDecl(id, std::move(qualifiedName), func) {}

RecordCtorDecl::RecordCtorDecl(std::string_view id,
                               QualifiedName qualifiedName,
                               std::vector<FunctionOverload> overloads)
	: FunctionDecl(id, std::move(qualifiedName), std::move(overloads)) {}

auto RecordCtorDecl::getDeclType() const -> DeclType {
	return DeclType::RecordCtor;
}

auto RecordCtorDecl::clone() const -> std::shared_ptr<FunctionDecl> {
	return std::make_shared<RecordCtorDecl>(
		getID(), getQualifiedName(), getOverloads()  //
	);
}
