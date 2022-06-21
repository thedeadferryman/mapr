//
// Created by kmeinkopf on 06.01.2022.
//

#pragma once

#include "view/function/functionOverload.hpp"
#include "view/name/qualifiedName.hpp"
#include "view/sparseDecl.hpp"

namespace mapr::view {

class FunctionDecl
	: public SparseDecl {
	std::vector<FunctionOverload> overloads;
	QualifiedName qualifiedName;

  public:
	FunctionDecl(std::string_view id,
	             QualifiedName qualifiedName,
	             const FunctionOverload& func);

	FunctionDecl(std::string_view id,
	             QualifiedName qualifiedName,
	             std::vector<FunctionOverload> overloads);

	auto addOverload(const FunctionOverload& func) -> bool;

	[[nodiscard]] auto addOverload(const FunctionOverload& func) const
		-> std::shared_ptr<FunctionDecl>;

	[[nodiscard]] virtual auto clone() const -> std::shared_ptr<FunctionDecl>;

	[[nodiscard]] auto getOverloads() const
		-> const std::vector<FunctionOverload>&;

	void dropOverloads();

	auto getSparseLocation() const -> std::shared_ptr<SparseSourceLoc> override;

	[[nodiscard]] auto acceptFilter(
		const std::shared_ptr<const config::FilterBase>& filter) const
		-> config::FilterResult override;

	[[nodiscard]] auto getQualifiedName() const -> QualifiedName override;

	[[nodiscard]] auto acceptSparseFilter(
		const std::shared_ptr<const config::FilterBase>& filter,
		config::SparseFilteringMode filteringMode) const
		-> std::shared_ptr<const DeclBase> override;

  private:
	[[nodiscard]] auto sharedThis() const
		-> std::shared_ptr<const FunctionDecl>;
};

auto operator<<(std::shared_ptr<FunctionDecl> overload,
                const FunctionOverload& decl)
	-> std::shared_ptr<const FunctionDecl>;

}  // namespace mapr::view