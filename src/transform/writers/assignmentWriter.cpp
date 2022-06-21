//
// Created by karl on 01.04.22.
//

#include "assignmentWriter.hpp"

using mapr::transform::AssignmentWriter;

auto AssignmentWriter::statement(std::unique_ptr<WriterBase> receiver,
                                 std::unique_ptr<WriterBase> value)
	-> std::unique_ptr<PlainStatementWriter> {
	return std::make_unique<PlainStatementWriter>(
		std::make_unique<AssignmentWriter>(  //
			std::move(receiver),
			std::move(value)  //
			));
}

AssignmentWriter::AssignmentWriter(std::unique_ptr<WriterBase> receiver,
                                   std::unique_ptr<WriterBase> value)
	: receiver(std::move(receiver))
	, value(std::move(value)) {}

void AssignmentWriter::apply(std::ostream& stream) const {
	stream << receiver << " = " << value;
}
