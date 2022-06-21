//
// Created by karl on 13.04.22.
//

#include <utility>

#include "operatorDecl.hpp"

using mapr::view::OperatorDecl;

OperatorDecl::OperatorDecl(std::string_view id,
                           QualifiedName qualifiedName,
                           const FunctionOverload& func,
                           Kind kind)
	: FunctionDecl(id, std::move(qualifiedName), func)
	, kind(kind) {}

OperatorDecl::OperatorDecl(std::string_view id,
                           QualifiedName qualifiedName,
                           std::vector<FunctionOverload> overloads,
                           Kind kind)
	: FunctionDecl(id, std::move(qualifiedName), std::move(overloads))
	, kind(kind) {}

auto OperatorDecl::getDeclType() const -> DeclType {
	return DeclType::OperatorOverload;
}

auto OperatorDecl::getOperatorKind() const -> OperatorDecl::Kind {
	return kind;
}
auto OperatorDecl::clone() const
	-> std::shared_ptr<FunctionDecl> {
	return std::make_shared<OperatorDecl>(
		getID(), getQualifiedName(), getOverloads(), kind  //
	);
}
