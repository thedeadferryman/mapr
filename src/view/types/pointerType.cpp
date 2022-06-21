//
// Created by kmeinkopf on 13.01.2022.
//

#include <sstream>

#include "pointerType.hpp"

using mapr::view::PointerType;
using mapr::view::TypeBase;

PointerType::PointerType(std::shared_ptr<TypeBase> pointee)
	: PointerType(clang::Qualifiers(), std::move(pointee)) {}

PointerType::PointerType(const clang::Qualifiers& quals,
                         std::shared_ptr<TypeBase> pointee)
	: TypeBase(TypeKind::Pointer, quals)
	, pointee(std::move(pointee)) {}

auto PointerType::getPointee() const -> const std::shared_ptr<TypeBase>& {
	return pointee;
}
auto PointerType::getPrettyName() const -> std::string {
	std::stringstream builder;

	if (getQualifiers().hasConst()) {
		builder << "const ";
	}

	builder << pointee->getPrettyName() << " *";

	return builder.str();
}
