//
// Created by karl on 21.03.22.
//

#include "plainStatementWriter.hpp"

using mapr::transform::PlainStatementWriter;

PlainStatementWriter::PlainStatementWriter(std::unique_ptr<WriterBase> body)
	: body(std::move(body)) {}

void PlainStatementWriter::apply(std::ostream& stream) const {
	stream << *body << ";\n";
}
