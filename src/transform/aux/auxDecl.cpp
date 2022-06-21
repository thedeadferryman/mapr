//
// Created by kmeinkopf on 19.01.2022.
//

#include "auxDecl.hpp"

#include "view/loc/sparseSourceLoc.hpp"
#include "view/name/qualifiedName.hpp"

using mapr::view::SourceLoc;
using mapr::view::SparseSourceLoc;

using mapr::transform::AuxDecl;

AuxDecl::AuxDecl(std::string_view declId)
	: view::DeclBase(declId, view::DeclType::Internal) {}

auto AuxDecl::getLocation() const -> std::shared_ptr<SourceLoc> {
	return std::make_shared<SparseSourceLoc>();
}

auto AuxDecl::getQualifiedName() const -> view::QualifiedName {
	return view::QualifiedName("<aux>") / getID();
}
