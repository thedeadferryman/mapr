//
// Created by karl on 31.03.22.
//

#pragma once

#include "view/function/functionDecl.hpp"

namespace mapr::view {

class RecordDtorDecl : public FunctionDecl {
  public:
	RecordDtorDecl(std::string_view id,
	               QualifiedName qualifiedName,
	               const FunctionOverload& func);

	RecordDtorDecl(std::string_view id,
	               QualifiedName qualifiedName,
	               std::vector<FunctionOverload> overloads);
	auto clone() const -> std::shared_ptr<FunctionDecl> override;
	[[nodiscard]] auto getDeclType() const -> DeclType override;

	[[nodiscard]] auto getOwnerName() const -> QualifiedName;
};

}  // namespace mapr::view
