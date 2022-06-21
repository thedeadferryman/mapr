//
// Created by kmeinkopf on 15.01.2022.
//

#include "typeResolver.hpp"

#include <clang/AST/TemplateBase.h>

#include "match/nameResolver.hpp"

#include "view/types/aliasType.hpp"
#include "view/types/builtinType.hpp"
#include "view/types/enumType.hpp"
#include "view/types/pointerType.hpp"
#include "view/types/recordType.hpp"
#include "view/types/referenceType.hpp"

#define RETURN_IF(_Expr) \
	if (auto resolvedType = (_Expr)) { \
		return resolvedType; \
	}

using mapr::match::TypeResolver;

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
		RETURN_IF(resolveAlias(type, astContext));
	} else if (type->isEnumeralType()) {
		RETURN_IF(resolveEnum(type, astContext));
	} else if (type->isRecordType()) {
		RETURN_IF(resolveRecord(type, astContext));
	} else {
		return std::make_shared<view::PointerType>(
			std::make_shared<view::BuiltinType>(
				clang::Qualifiers(), "void", view::BuiltinType::Variant::Void));
		UNREACHABLE();
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

auto TypeResolver::resolveAlias(const clang::QualType& type,
                                const clang::ASTContext* astContext)
	-> std::shared_ptr<view::TypeBase> {
	if (const auto* alias = type->getAs<clang::TypedefType>()) {
		auto aliasName =
			NameResolver::resolveNamedDeclName(alias->getDecl(), astContext);
		auto addressedType = alias->getDecl()->getUnderlyingType();

		if (auto addressedDecl = addressedType->getAsRecordDecl()) {
			auto addressedName =
				NameResolver::resolveNamedDeclName(addressedDecl, astContext);

			if (!addressedName.isValid()) {
				return resolveRecord(addressedType, astContext, aliasName);
			}
		}

		return std::make_shared<view::AliasType>(
			type.getQualifiers(),
			aliasName,
			resolve(alias->getDecl()->getUnderlyingType(), astContext));
	}
	if (const auto* alias = type->getAs<clang::TemplateSpecializationType>()) {
		if (auto* tagDecl = alias->getAsTagDecl()) {
			return std::make_shared<view::AliasType>(
				type.getQualifiers(),
				NameResolver::resolveNamedDeclName(tagDecl, astContext),
				resolve(alias->getAliasedType(), astContext));
		}

		return resolve(alias->getAliasedType(), astContext);
	}

	return nullptr;
}
auto TypeResolver::resolveEnum(const clang::QualType& type,
                               const clang::ASTContext* context)
	-> std::shared_ptr<view::TypeBase> {
	if (const auto* enumType = type->getAs<clang::EnumType>()) {
		return std::make_shared<view::EnumType>(
			type.getQualifiers(),
			NameResolver::resolveDeclName(enumType->getDecl(), context),
			enumType->isScopedEnumeralType());
	}

	return nullptr;
}
auto TypeResolver::resolveRecord(
	const clang::QualType& type,
	const clang::ASTContext* context,
	std::optional<view::QualifiedName> overrideName)
	-> std::shared_ptr<view::TypeBase> {
	if (const auto* recordType = type->getAs<clang::RecordType>()) {
		auto name = overrideName.value_or(
			NameResolver::resolveDeclName(recordType->getDecl(), context));

		return std::make_shared<view::RecordType>(type.getQualifiers(), name);
	}

	return nullptr;
}
