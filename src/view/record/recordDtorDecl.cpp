//
// Created by karl on 31.03.22.
//

#include <utility>

#include "recordDtorDecl.hpp"

using mapr::view::RecordDtorDecl;

RecordDtorDecl::RecordDtorDecl(std::string_view id,
                               QualifiedName qualifiedName,
                               const FunctionOverload& func)
	: FunctionDecl(id, std::move(qualifiedName), func) {}

RecordDtorDecl::RecordDtorDecl(std::string_view id,
                               mapr::view::QualifiedName qualifiedName,
                               std::vector<FunctionOverload> overloads)
	: FunctionDecl(id, std::move(qualifiedName), std::move(overloads)) {}

auto RecordDtorDecl::getDeclType() const -> DeclType {
	return DeclType::RecordDtor;
}
auto RecordDtorDecl::getOwnerName() const -> QualifiedName {
	return getQualifiedName().parent();
}
auto RecordDtorDecl::clone() const -> std::shared_ptr<FunctionDecl> {
	return std::make_shared<RecordDtorDecl>(
		getID(), getQualifiedName(), getOverloads()  //
	);
}
