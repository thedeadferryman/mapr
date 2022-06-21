//
// Created by karl on 13.04.22.
//

#pragma once

#include "view/record/recordMethodDecl.hpp"

namespace mapr::view {

class RecordOperatorDecl : public RecordMethodDecl {
  public:
	using Kind = clang::OverloadedOperatorKind;

  private:
	Kind kind;

  public:
	RecordOperatorDecl(std::string_view id,
	                   QualifiedName qualifiedName,
	                   const FunctionOverload& func,
	                   Kind kind);

	RecordOperatorDecl(std::string_view id,
	                   mapr::view::QualifiedName qualifiedName,
	                   std::vector<FunctionOverload> overloads,
	                   RecordOperatorDecl::Kind kind);
	auto clone() const -> std::shared_ptr<FunctionDecl> override;
	[[nodiscard]] auto getDeclType() const -> DeclType override;

	[[nodiscard]] auto getOperatorKind() const -> Kind;
};

}  // namespace mapr::view
