//
// Created by kmeinkopf on 06.01.2022.
//

#include "functionDecl.hpp"

#include "view/loc/sparseSourceLoc.hpp"

#include "util/stringBuilder.hpp"

#include "filtering/filters/filterBase.hpp"

using mapr::view::FunctionDecl;
using mapr::view::FunctionOverload;

FunctionDecl::FunctionDecl(std::string_view id,
                           QualifiedName qualifiedName,
                           const FunctionOverload& func)
	: SparseDecl(id, DeclType::Function)
	, overloads {func}
	, qualifiedName(std::move(qualifiedName)) {}

FunctionDecl::FunctionDecl(std::string_view id,
                           QualifiedName qualifiedName,
                           std::vector<FunctionOverload> overloads)
	: SparseDecl(id, DeclType::Function)
	, overloads(std::move(overloads))
	, qualifiedName(std::move(qualifiedName)) {}

auto FunctionDecl::addOverload(const FunctionOverload& func) -> bool {
	auto found = std::find(std::begin(overloads), std::end(overloads), func);
	if (found != std::end(overloads)) {
		return false;
	}

	overloads.emplace_back(func);

	return true;
}

auto FunctionDecl::getOverloads() const
	-> const std::vector<FunctionOverload>& {
	return overloads;
}

void FunctionDecl::dropOverloads() {
	overloads.clear();
}

auto FunctionDecl::getQualifiedName() const -> QualifiedName {
	return qualifiedName;
}

auto mapr::view::FunctionDecl::getSparseLocation() const
	-> std::shared_ptr<SparseSourceLoc> {
	auto locs = std::vector<std::shared_ptr<SingleSourceLoc>>();

	std::transform(std::begin(overloads),
	               std::end(overloads),
	               std::back_inserter(locs),
	               [](const FunctionOverload& overload) {
					   return overload.getLocation();
				   });

	return std::make_shared<SparseSourceLoc>(std::move(locs));
}

auto FunctionDecl::acceptFilter(
	const std::shared_ptr<const config::FilterBase>& filter) const
	-> mapr::config::FilterResult {
	return filter->checkFunctionDecl(sharedThis());
}

auto FunctionDecl::acceptSparseFilter(
	const std::shared_ptr<const config::FilterBase>& filter,
	config::SparseFilteringMode filteringMode) const
	-> std::shared_ptr<const DeclBase> {
	return filter->filterFunctionDecl(sharedThis(), filteringMode);
}

auto FunctionDecl::sharedThis() const -> std::shared_ptr<const FunctionDecl> {
	if (auto funDecl =
	        std::dynamic_pointer_cast<const FunctionDecl>(shared_from_this())) {
		return funDecl;
	}

	UNREACHABLE();
}

auto FunctionDecl::clone() const -> std::shared_ptr<FunctionDecl> {
	return std::make_shared<FunctionDecl>(getID(), qualifiedName, overloads);
}

auto FunctionDecl::addOverload(const FunctionOverload& func) const
	-> std::shared_ptr<FunctionDecl> {
	auto newFunc = clone();

	newFunc->addOverload(func);

	return newFunc;
}

auto mapr::view::operator<<(std::shared_ptr<FunctionDecl> overload,
                              const FunctionOverload& decl)
	-> std::shared_ptr<const FunctionDecl> {
	overload->addOverload(decl);
	return overload;
}
