//
// Created by karl on 31.03.22.
//

#include "pointerTypeWriter.hpp"

using mapr::transform::PointerTypeWriter;

PointerTypeWriter::PointerTypeWriter(const clang::Qualifiers& quals,
                                     std::unique_ptr<WriterBase> pointee)
	: qualifiers(quals)
	, pointee(std::move(pointee)) {}

void PointerTypeWriter::apply(std::ostream& stream) const {
	if (qualifiers.hasConst()) {
		stream << "const ";
	}
	if (qualifiers.hasVolatile()) {
		stream << "volatile ";
	}

	stream << pointee << " *";
}
