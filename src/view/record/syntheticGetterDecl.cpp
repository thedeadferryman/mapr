//
// Created by karl on 11.04.22.
//

#include "syntheticGetterDecl.hpp"

using mapr::view::SyntheticGetterDecl;

SyntheticGetterDecl::SyntheticGetterDecl(QualifiedName qualifiedName,
                                         const FunctionOverload& overload)
	: FunctionDecl(qualifiedName.str(), std::move(qualifiedName), overload) {}

SyntheticGetterDecl::SyntheticGetterDecl(
	std::string_view id,
	mapr::view::QualifiedName qualifiedName,
	std::vector<FunctionOverload> overloads)
	: FunctionDecl(id, std::move(qualifiedName), std::move(overloads)) {}

auto SyntheticGetterDecl::getDeclType() const -> DeclType {
	return DeclType::SyntheticGetter;
}

auto SyntheticGetterDecl::clone() const -> std::shared_ptr<FunctionDecl> {
	return std::make_shared<SyntheticGetterDecl>(
		getID(), getQualifiedName(), getOverloads()  //
	);
}
