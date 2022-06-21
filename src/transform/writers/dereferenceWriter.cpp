//
// Created by karl on 01.04.22.
//

#include "dereferenceWriter.hpp"

using mapr::transform::DereferenceWriter;

DereferenceWriter::DereferenceWriter(std::unique_ptr<WriterBase> value)
	: value(std::move(value)) {}

void DereferenceWriter::apply(std::ostream& stream) const {
	stream << "(*(" << value << "))";
}
