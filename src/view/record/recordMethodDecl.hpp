//
// Created by karl on 31.03.22.
//

#pragma once

#include "view/function/functionDecl.hpp"

namespace mapr::view {

class RecordMethodDecl : public FunctionDecl {
  public:
	RecordMethodDecl(std::string_view id,
	                 QualifiedName qualifiedName,
	                 const FunctionOverload& func);

	RecordMethodDecl(std::string_view id,
	                 QualifiedName qualifiedName,
	                 std::vector<FunctionOverload> overloads);

	[[nodiscard]] auto clone() const -> std::shared_ptr<FunctionDecl> override;

	[[nodiscard]] auto getDeclType() const -> DeclType override;
};

}  // namespace mapr::view
