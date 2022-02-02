//
// Created by kmeinkopf on 24.01.2022.
//

#include "typeRequest.hpp"

using kodgen::transform::TypeRequest;

TypeRequest::TypeRequest(std::shared_ptr<view::TypeBase> type)
	: DependencyRequest(DependencyKind::Type)
	, type(std::move(type)) {}

auto TypeRequest::getType() const -> std::shared_ptr<view::TypeBase> {
	return type;
}
