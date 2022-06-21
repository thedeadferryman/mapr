//
// Created by karl on 11.04.22.
//

#pragma once

#include "view/function/functionDecl.hpp"

namespace mapr::view {

class SyntheticGetterDecl : public FunctionDecl {
  public:
	SyntheticGetterDecl(QualifiedName qualifiedName,
	                    const FunctionOverload& overload);

	SyntheticGetterDecl(std::string_view id,
	                    mapr::view::QualifiedName qualifiedName,
	                    std::vector<FunctionOverload> overloads);

	auto clone() const -> std::shared_ptr<FunctionDecl> override;

	[[nodiscard]] auto getDeclType() const -> DeclType override;
};

}  // namespace mapr::view
