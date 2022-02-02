//
// Created by kmeinkopf on 19.01.2022.
//

#include "typeNameTransformer.hpp"

#include "transform/exceptions/unsupportedTypeException.hpp"
#include "transform/name/namespacedNameTransformer.hpp"

#include "util/matchType.hpp"

using kodgen::transform::TypeNameTransformer;
using kodgen::transform::UnsupportedTypeException;
using kodgen::util::MatchType;

constexpr auto unsupportedType =
	[](const std::shared_ptr<kodgen::view::TypeBase>& type) -> std::string {
	throw UnsupportedTypeException(type->getKind(), type->getPrettyName());
};

auto TypeNameTransformer::getTypeName(std::shared_ptr<view::TypeBase> type)
	-> std::string {
	return MatchType::matchType<std::string>(
		type,  //
		[](const std::shared_ptr<view::BuiltinType>& builtin) {
			return getBuiltinTypeName(builtin);
		},
		[](const std::shared_ptr<view::PointerType>& ptr) {
			return getPointerTypeName(ptr);
		},
		[](const std::shared_ptr<view::ReferenceType>& ref) {
			return getDeclarationReferenceTypeName(ref);
		},
		[](const std::shared_ptr<view::AliasType>& alias) {
			return getTypeName(alias->getSource());
		},
		[](const std::shared_ptr<view::EnumType>& enumType) {
			return getEnumTypeName(enumType);
		},
		unsupportedType);
}

auto TypeNameTransformer::getUnqualifiedTypeName(
	std::shared_ptr<view::TypeBase> type) -> std::string {
	return std::string();
}

auto TypeNameTransformer::getOverloadSlug(std::shared_ptr<view::TypeBase> type)
	-> std::string {
	auto builder = std::stringstream();

	builder << getUnqualifiedOverloadSlug(type);

	if (type->getQualifiers().hasVolatile()) {
		builder << "V";
	}

	if (type->getQualifiers().hasConst()) {
		builder << "C";
	}

	return builder.str();
}

auto TypeNameTransformer::getUnqualifiedOverloadSlug(
	std::shared_ptr<view::TypeBase> type) -> std::string {
	return MatchType::matchType<std::string>(
		type,  //
		[](const std::shared_ptr<view::PointerType>& ptr) -> std::string {
			return getOverloadSlug(ptr->getPointee()) + "P";
		},
		[](const std::shared_ptr<view::ReferenceType>& ref) -> std::string {
			return getOverloadSlug(ref->dereference())
				+ std::string(referenceKindSuffix(ref->getReferenceKind()));
		},
		[](const std::shared_ptr<view::AliasType>& alias) -> std::string {
			return alias->getQualifiedName();
		},
		[](const std::shared_ptr<view::BuiltinType>& builtin) -> std::string {
			return getBuiltinOverloadSlug(builtin);
		},
		unsupportedType);
}

auto TypeNameTransformer::referenceKindSuffix(kodgen::view::ReferenceKind kind)
	-> std::string_view {
	switch (kind) {
		case kodgen::view::ReferenceKind::LVALUE:
			return "LV";
		case kodgen::view::ReferenceKind::RVALUE:
			return "RV";
		default:
			return "R";
	}
}

auto TypeNameTransformer::getBuiltinOverloadSlug(
	const std::shared_ptr<view::BuiltinType>& builtinType) -> std::string {
	switch (builtinType->getVariant()) {
		case clang::BuiltinType::Void:
			return "void";
		case clang::BuiltinType::NullPtr:
			return "null";

		case clang::BuiltinType::Bool:
			return "bool";

		case clang::BuiltinType::Char_U:
		case clang::BuiltinType::Char_S:
			return "cc";
		case clang::BuiltinType::SChar:
			return "sc";
		case clang::BuiltinType::UChar:
			return "uc";

		case clang::BuiltinType::WChar_U:
		case clang::BuiltinType::WChar_S:
			return "wc";

		case clang::BuiltinType::Char8:
			return "c8";
		case clang::BuiltinType::Char16:
			return "c16";
		case clang::BuiltinType::Char32:
			return "c32";

		case clang::BuiltinType::UShort:
			return "us";
		case clang::BuiltinType::UInt:
			return "ui";
		case clang::BuiltinType::ULong:
			return "ul";
		case clang::BuiltinType::ULongLong:
			return "ull";
		case clang::BuiltinType::UInt128:
			return "u128";

		case clang::BuiltinType::Short:
			return "si";
		case clang::BuiltinType::Int:
			return "ii";
		case clang::BuiltinType::Long:
			return "li";
		case clang::BuiltinType::LongLong:
			return "lli";
		case clang::BuiltinType::Int128:
			return "i128";

		case clang::BuiltinType::Float:
			return "lf";
		case clang::BuiltinType::Double:
			return "ld";
		case clang::BuiltinType::LongDouble:
			return "lld";
		case clang::BuiltinType::Float16:
			return "f16";
		case clang::BuiltinType::Float128:
			return "f128";

		default:
			throw UnsupportedTypeException(builtinType->getVariant(),
			                               builtinType->getDisplayName());
	}
}

auto TypeNameTransformer::getBuiltinTypeName(
	const std::shared_ptr<view::BuiltinType>& builtinType) -> std::string {
	auto builder = std::stringstream();

	if (builtinType->getQualifiers().hasVolatile()) {
		builder << "volatile ";
	}

	if (builtinType->getQualifiers().hasConst()) {
		builder << "const ";
	}

	builder
		<< builtinType->getDisplayName();  // TODO Review `displayName` behavior

	return builder.str();
}

auto TypeNameTransformer::getPointerTypeName(
	const std::shared_ptr<view::PointerType>& pointerType) -> std::string {
	auto builder = std::stringstream();

	builder << getTypeName(pointerType->getPointee()) << " *";

	if (pointerType->getQualifiers().hasVolatile()) {
		builder << " volatile";
	}

	if (pointerType->getQualifiers().hasConst()) {
		builder << " const";
	}

	return builder.str();
}

/// We don't have references in C, so we're using pointers instead
auto TypeNameTransformer::getDeclarationReferenceTypeName(
	const std::shared_ptr<view::ReferenceType>& referenceType) -> std::string {
	return getTypeName(referenceType->dereference()) + " *";
}

auto TypeNameTransformer::getEnumTypeName(
	const std::shared_ptr<view::EnumType>& enumType) -> std::string {
	return NamespacedNameTransformer::getName(enumType->getName());
}
