//
// Created by kmeinkopf on 13.01.2022.
//

#include "typeBase.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <llvm/Support/raw_ostream.h>

#include "view/types/aliasType.hpp"
#include "view/types/builtinType.hpp"
#include "view/types/pointerType.hpp"
#include "view/types/referenceType.hpp"

using mapr::view::TypeBase;

TypeBase::TypeBase(mapr::view::TypeKind kind,
                   const clang::Qualifiers& qualifiers)
	: qualifiers(qualifiers)
	, kind(kind) {}

auto TypeBase::getKind() const -> TypeKind {
	return kind;
}

auto TypeBase::getQualifiers() const -> const clang::Qualifiers& {
	return qualifiers;
}

auto TypeBase::isVoid() const -> bool {
	return false;
}

