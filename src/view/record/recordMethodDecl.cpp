//
// Created by karl on 31.03.22.
//

#include <utility>

#include "recordMethodDecl.hpp"

using mapr::view::RecordMethodDecl;

RecordMethodDecl::RecordMethodDecl(std::string_view id,
                                   QualifiedName qualifiedName,
                                   const FunctionOverload& func)
	: FunctionDecl(id, std::move(qualifiedName), func) {}

RecordMethodDecl::RecordMethodDecl(std::string_view id,
                                   mapr::view::QualifiedName qualifiedName,
                                   std::vector<FunctionOverload> overloads)
	: FunctionDecl(id, std::move(qualifiedName), std::move(overloads)) {}

auto RecordMethodDecl::getDeclType() const -> DeclType {
	return DeclType::RecordMethod;
}

auto RecordMethodDecl::clone() const -> std::shared_ptr<FunctionDecl> {
	return std::make_shared<RecordMethodDecl>(
		getID(), getQualifiedName(), getOverloads()  //
	);
}
