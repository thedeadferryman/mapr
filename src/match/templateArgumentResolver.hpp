//
// Created by karl on 13.05.22.
//

#pragma once

#include <clang/AST/DeclTemplate.h>

#include "view/name/templates/templateArgumentBase.hpp"

namespace mapr::match {

class TemplateArgumentResolver {
  public:
	[[nodiscard]] static auto forClangArgument(clang::TemplateArgument argument,
	                                           const clang::ASTContext* context)
		-> std::shared_ptr<view::TemplateArgumentBase>;
};

}  // namespace mapr::match
