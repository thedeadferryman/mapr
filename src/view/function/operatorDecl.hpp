//
// Created by karl on 13.04.22.
//

#pragma once

#include "view/function/functionDecl.hpp"

namespace mapr::view {

class OperatorDecl : public FunctionDecl {
  public:
	using Kind = clang::OverloadedOperatorKind;

  private:
	Kind kind;

  public:
	OperatorDecl(std::string_view id,
	             QualifiedName qualifiedName,
	             const FunctionOverload& func,
	             Kind kind);

	OperatorDecl(std::string_view id,
	             QualifiedName qualifiedName,
	             std::vector<FunctionOverload> overloads,
	             Kind kind);

	[[nodiscard]] auto getDeclType() const -> DeclType override;

	[[nodiscard]] auto getOperatorKind() const -> Kind;

	[[nodiscard]] auto clone() const -> std::shared_ptr<FunctionDecl> override;
};

}  // namespace mapr::view
