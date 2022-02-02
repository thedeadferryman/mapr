//
// Created by kmeinkopf on 19.01.2022.
//

#include "auxDecl.hpp"

using kodgen::transform::AuxDecl;

AuxDecl::AuxDecl(std::string_view declId)
	: view::DeclBase(declId, view::DeclType::Internal) {}
