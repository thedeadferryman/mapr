//
// Created by kmeinkopf on 14.03.22.
//

#include <utility>

#include "recordType.hpp"

#include "view/types/pointerType.hpp"

using mapr::view::RecordType;

auto RecordType::makePointer(const QualifiedName& qualifiedName,
                             ConstMode constMode)
	-> std::shared_ptr<PointerType> {
	auto ptrQualifiers = clang::Qualifiers();
	auto valueQualifiers = clang::Qualifiers();

	if ((constMode & ConstMode::ConstPointer) > 0) {
		ptrQualifiers.addConst();
	}
	if ((constMode & ConstMode::ConstValue) > 0) {
		valueQualifiers.addConst();
	}

	return std::make_shared<PointerType>(
		ptrQualifiers,
		std::make_shared<RecordType>(valueQualifiers, qualifiedName));
}

RecordType::RecordType(const clang::Qualifiers& qualifiers,
                       QualifiedName nsName)
	: TypeBase(TypeKind::Record, qualifiers)
	, qualifiedName(std::move(nsName)) {}

auto RecordType::getPrettyName() const -> std::string {
	return qualifiedName.str();
}

auto RecordType::getName() const -> const mapr::view::QualifiedName& {
	return qualifiedName;
}

auto RecordType::asPointer(mapr::view::ConstMode constMode) const
	-> std::shared_ptr<PointerType> {
	return makePointer(qualifiedName, constMode);
}
