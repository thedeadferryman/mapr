//
// Created by karl on 11.04.22.
//

#pragma once

#include "view/function/functionDecl.hpp"

namespace mapr::view {

class SyntheticSetterDecl : public FunctionDecl {
  public:
	SyntheticSetterDecl(QualifiedName qualifiedName,
	                    const FunctionOverload& overload);

	SyntheticSetterDecl(std::string_view id,
	                    QualifiedName qualifiedName,
	                    std::vector<FunctionOverload> overloads);

	[[nodiscard]] auto clone() const -> std::shared_ptr<FunctionDecl> override;

	[[nodiscard]] auto getDeclType() const -> DeclType override;
};

}  // namespace mapr::view
