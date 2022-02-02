//
// Created by kmeinkopf on 15.01.2022.
//

#include "typeResolver.hpp"

#include "match/nameResolver.hpp"

#include "view/types/aliasType.hpp"
#include "view/types/builtinType.hpp"
#include "view/types/enumType.hpp"
#include "view/types/pointerType.hpp"
#include "view/types/referenceType.hpp"

#define RETURN_IF(_Expr) \
	if (auto resolvedType = (_Expr)) { \
		return resolvedType; \
	}

using kodgen::match::TypeResolver;

auto TypeResolver::resolve(const clang::QualType& type,
                           const clang::ASTContext* astContext)
	-> std::shared_ptr<view::TypeBase> {
	if (type->isBuiltinType()) {
		RETURN_IF(resolveBuiltin(type, astContext));
	} else if (type->isPointerType()) {
		RETURN_IF(resolvePointer(type, astContext));
	} else if (type->isReferenceType()) {
		RETURN_IF(resolveReference(type, astContext));
	} else if (type->isTypedefNameType()) {
		RETURN_IF(resolveTypedef(type, astContext));
	} else if (type->isEnumeralType()) {
		RETURN_IF(resolveEnum(type, astContext));
	} else if (type->isRecordType()) {
		llvm::errs() << "record: " << type.getAsString() << "\n";

		return std::make_shared<view::BuiltinType>(
			type.getQualifiers(), "_Record", clang::BuiltinType::Kind::Void);
	}

	return nullptr;
}

auto TypeResolver::resolveBuiltin(const clang::QualType& type,
                                  const clang::ASTContext* astContext)
	-> std::shared_ptr<view::TypeBase> {
	if (const auto* builtinType = type->getAs<clang::BuiltinType>()) {
		return std::make_shared<view::BuiltinType>(
			type.getQualifiers(),
			builtinType->getName(astContext->getPrintingPolicy()),
			builtinType->getKind());
	}

	return nullptr;
}

auto TypeResolver::resolvePointer(const clang::QualType& type,
                                  const clang::ASTContext* astContext)
	-> std::shared_ptr<view::TypeBase> {
	if (const auto* pointerType = type->getAs<clang::PointerType>()) {
		return std::make_shared<view::PointerType>(
			type.getQualifiers(),
			resolve(pointerType->getPointeeType(), astContext));
	}

	return nullptr;
}

auto TypeResolver::resolveReference(const clang::QualType& type,
                                    const clang::ASTContext* astContext)
	-> std::shared_ptr<view::TypeBase> {
	auto refKind =
		(type->isLValueReferenceType()
	         ? view::ReferenceKind::LVALUE
	         : (type->isRValueReferenceType() ? view::ReferenceKind::RVALUE
	                                          : view::ReferenceKind::UNKNOWN));

	return std::make_shared<view::ReferenceType>(
		type.getQualifiers(),
		resolve(type.getNonReferenceType(), astContext),
		refKind);
}

auto TypeResolver::resolveTypedef(const clang::QualType& type,
                                  const clang::ASTContext* astContext)
	-> std::shared_ptr<view::TypeBase> {
	if (const auto* alias = type->getAs<clang::TypedefType>()) {
		return std::make_shared<view::AliasType>(
			type.getQualifiers(),
			type.getAsString(),
			resolve(alias->getDecl()->getUnderlyingType(), astContext));
	}

	return nullptr;
}
auto TypeResolver::resolveEnum(const clang::QualType& type,
                               const clang::ASTContext* /*unused*/)
	-> std::shared_ptr<view::TypeBase> {
	if (const auto* enumType = type->getAs<clang::EnumType>()) {
		return std::make_shared<view::EnumType>(
			type.getQualifiers(),
			NameResolver::resolveDeclName(enumType->getDecl()),
			enumType->isScopedEnumeralType());
	}

	return nullptr;
}
