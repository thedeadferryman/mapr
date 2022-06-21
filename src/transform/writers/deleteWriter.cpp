//
// Created by karl on 01.04.22.
//

#include "deleteWriter.hpp"

using mapr::transform::DeleteWriter;

DeleteWriter::DeleteWriter(std::unique_ptr<WriterBase> object)
	: object(std::move(object)) {}

void DeleteWriter::apply(std::ostream& stream) const {
	stream << "delete (";

	stream << object << ")";
}