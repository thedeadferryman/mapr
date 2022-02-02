//
// Created by kmeinkopf on 06.01.2022.
//

#pragma once

#include "view/function/functionOverload.hpp"
#include "view/namespacedName.hpp"

namespace kodgen::view {

class FunctionDecl : public DeclBase {
	std::vector<FunctionOverload> overloads;
	NamespacedName qualifiedName;

  public:
	explicit FunctionDecl(std::string_view id,
	                      NamespacedName qualifiedName,
	                      const FunctionOverload& func);

	void addOverload(const FunctionOverload& func);

	[[nodiscard]] auto getOverloads() const
		-> const std::vector<FunctionOverload>&;

	[[nodiscard]] auto getQualifiedName() const -> const NamespacedName&;
};

auto operator<<(std::shared_ptr<FunctionDecl> overload,
                const FunctionOverload& decl) -> std::shared_ptr<FunctionDecl>;

}  // namespace kodgen::view