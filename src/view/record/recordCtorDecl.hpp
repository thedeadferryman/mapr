//
// Created by karl on 31.03.22.
//

#pragma once

#include "view/function/functionDecl.hpp"

namespace mapr::view {

class RecordCtorDecl : public FunctionDecl {
  public:
	RecordCtorDecl(std::string_view id,
	               QualifiedName qualifiedName,
	               const FunctionOverload& func);

	RecordCtorDecl(std::string_view id,
	               QualifiedName qualifiedName,
	               std::vector<FunctionOverload> overloads);

	[[nodiscard]] auto getDeclType() const -> DeclType override;

	auto clone() const -> std::shared_ptr<FunctionDecl> override;
};

}  // namespace mapr::view
