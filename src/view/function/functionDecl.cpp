//
// Created by kmeinkopf on 06.01.2022.
//

#include "functionDecl.hpp"

#include "util/stringBuilder.hpp"

using kodgen::view::DeclBase;
using kodgen::view::FunctionDecl;
using kodgen::view::FunctionOverload;

FunctionDecl::FunctionDecl(std::string_view id,
                           NamespacedName qualifiedName,
                           const FunctionOverload& func)
	: DeclBase(id, DeclType::FunctionOverload)
	, overloads {func}
	, qualifiedName(std::move(qualifiedName)) {}

void FunctionDecl::addOverload(const FunctionOverload& func) {
	overloads.push_back(func);
}

auto FunctionDecl::getOverloads() const
	-> const std::vector<FunctionOverload>& {
	return overloads;
}

auto FunctionDecl::getQualifiedName() const -> const NamespacedName& {
	return qualifiedName;
}

auto kodgen::view::operator<<(std::shared_ptr<FunctionDecl> overload,
                              const FunctionOverload& decl)
	-> std::shared_ptr<FunctionDecl> {
	overload->addOverload(decl);
	return overload;
}
