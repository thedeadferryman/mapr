//
// Created by kmeinkopf on 20.11.2021.
//

#include "declBase.hpp"

#include "filtering/filters/filterBase.hpp"

using mapr::view::DeclBase;

DeclBase::DeclBase(std::string_view id, DeclType declType)
	: id(id)
	, declType(declType) {};

auto DeclBase::getID() const -> const std::string& {
	return id;
}

auto DeclBase::getDeclType() const -> DeclType {
	return declType;
}

auto DeclBase::acceptFilter(const std::shared_ptr<const config::FilterBase>& filter)
	const -> mapr::config::FilterResult {
	return filter->checkDecl(shared_from_this());
}

auto DeclBase::acceptSparseFilter(
	const std::shared_ptr<const config::FilterBase>& /*filter*/,
	mapr::config::SparseFilteringMode /*filteringMode*/) const
	-> std::shared_ptr<const DeclBase> {
	return shared_from_this();
}
