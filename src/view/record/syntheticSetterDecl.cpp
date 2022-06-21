//
// Created by karl on 11.04.22.
//

#include "syntheticSetterDecl.hpp"

using mapr::view::SyntheticSetterDecl;

SyntheticSetterDecl::SyntheticSetterDecl(QualifiedName qualifiedName,
                                         const FunctionOverload& overload)
	: FunctionDecl(
		qualifiedName.str() + "=", std::move(qualifiedName), overload) {}

auto SyntheticSetterDecl::getDeclType() const -> DeclType {
	return DeclType::SyntheticSetter;
}
SyntheticSetterDecl::SyntheticSetterDecl(
	std::string_view id,
	QualifiedName qualifiedName,
	std::vector<FunctionOverload> overloads)
	: FunctionDecl(id, std::move(qualifiedName), std::move(overloads)) {}

auto SyntheticSetterDecl::clone() const -> std::shared_ptr<FunctionDecl> {
	return std::make_shared<SyntheticSetterDecl>(
		getID(), getQualifiedName(), getOverloads()  //
	);;
}
