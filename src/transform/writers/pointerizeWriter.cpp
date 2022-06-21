//
// Created by karl on 15.05.22.
//

#include "pointerizeWriter.hpp"

using mapr::transform::PointerizeWriter;

PointerizeWriter::PointerizeWriter(std::unique_ptr<WriterBase> expr)
	: expr(std::move(expr)) {}

void PointerizeWriter::apply(std::ostream& stream) const {
	stream << "(&(" << expr << "))";
}
