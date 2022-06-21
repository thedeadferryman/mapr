//
// Created by kmeinkopf on 13.01.2022.
//

#include <sstream>

#include "referenceType.hpp"

using mapr::view::ReferenceType;

ReferenceType::ReferenceType(const clang::Qualifiers& quals,
                             std::shared_ptr<TypeBase> referenced,
                             ReferenceKind variant)
	: TypeBase(TypeKind::Reference, quals)
	, referenceKind(variant)
	, referenced(std::move(referenced)) {}

auto ReferenceType::getReferenceKind() const -> ReferenceKind {
	return referenceKind;
}

auto ReferenceType::dereference() const -> std::shared_ptr<TypeBase> {
	return referenced;
}
auto ReferenceType::getPrettyName() const -> std::string {
	std::stringstream builder;

	if (getQualifiers().hasConst()) {
		builder << "const ";
	}

	builder << referenced->getPrettyName()
			<< (referenceKind == ReferenceKind::LVALUE ? " &" : " &&");

	return builder.str();
}
