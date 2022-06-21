//
// Created by karl on 21.03.22.
//

#include "returnStatementWriter.hpp"

using mapr::transform::ReturnStatementWriter;

void ReturnStatementWriter::apply(std::ostream& stream) const {
	stream << "return";

	if (value != nullptr) {
		stream << ' ' << *value;
	}

	stream << ";\n";
}

ReturnStatementWriter::ReturnStatementWriter(std::unique_ptr<WriterBase> value)
	: value(std::move(value)) {}
