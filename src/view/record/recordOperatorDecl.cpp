//
// Created by karl on 13.04.22.
//

#include "recordOperatorDecl.hpp"

using mapr::view::RecordOperatorDecl;

RecordOperatorDecl::RecordOperatorDecl(std::string_view id,
                                       QualifiedName qualifiedName,
                                       const FunctionOverload& func,
                                       Kind kind)
	: RecordMethodDecl(id, std::move(qualifiedName), func)
	, kind(kind) {}

auto RecordOperatorDecl::getDeclType() const -> DeclType {
	return DeclType::RecordOperatorOverload;
}

auto RecordOperatorDecl::getOperatorKind() const -> RecordOperatorDecl::Kind {
	return kind;
}

RecordOperatorDecl::RecordOperatorDecl(
	std::string_view id,
	mapr::view::QualifiedName qualifiedName,
	std::vector<FunctionOverload> overloads,
	RecordOperatorDecl::Kind kind)
	: RecordMethodDecl(id, std::move(qualifiedName), std::move(overloads))
	, kind(kind) {}

auto RecordOperatorDecl::clone() const -> std::shared_ptr<FunctionDecl> {
	return std::make_shared<RecordOperatorDecl>(
		getID(), getQualifiedName(), getOverloads(), kind  //
	);
}
