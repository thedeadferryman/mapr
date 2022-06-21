//
// Created by karl on 25.05.22.
//

#include "sparseDecl.hpp"

using mapr::view::SparseDecl;

auto SparseDecl::getLocation() const -> std::shared_ptr<SourceLoc> {
	return getSparseLocation();
}

SparseDecl::SparseDecl(std::string_view id, DeclType kind)
	: DeclBase(id, kind) {}
