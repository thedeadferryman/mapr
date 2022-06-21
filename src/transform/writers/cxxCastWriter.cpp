//
// Created by karl on 22.03.22.
//

#include "cxxCastWriter.hpp"

#include "util/macro.hpp"

using mapr::transform::CXXCastWriter;

CXXCastWriter::CXXCastWriter(std::unique_ptr<WriterBase> value,
                             std::unique_ptr<WriterBase> type,
                             CastStyle style)
	: value(std::move(value))
	, type(std::move(type))
	, style(style) {}

void CXXCastWriter::apply(std::ostream& stream) const {
	stream << "(";

	switch (style) {
		case CastStyle::StaticCast:
			stream << "static_cast";
			break;
		case CastStyle::ConstCast:
			stream << "const_cast";
			break;
		case CastStyle::DynamicCast:
			stream << "dynamic_cast";
			break;
		case CastStyle::ReinterpretCast:
			stream << "reinterpret_cast";
			break;
		default:
			UNREACHABLE();
	}

	stream << "<" << *type << ">(" << *value << "))";
}
