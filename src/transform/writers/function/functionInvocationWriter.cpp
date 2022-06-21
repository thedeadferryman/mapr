//
// Created by karl on 21.03.22.
//

#include "functionInvocationWriter.hpp"

using mapr::transform::FunctionInvocationWriter;

void f() {}

FunctionInvocationWriter::FunctionInvocationWriter(
	std::unique_ptr<WriterBase> name,
	std::vector<std::unique_ptr<WriterBase>> arguments,
	InvocationStyle style)
	: name(std::move(name))
	, arguments(std::move(arguments))
	, style(style) {}

void FunctionInvocationWriter::apply(std::ostream& stream) const {
	if (style == InvocationStyle::ParensAround) {
		stream << '(';
	}

	stream << *name << '(';

	if (!arguments.empty()) {
		stream << *arguments.front();

		for (auto it = std::begin(arguments) + 1; it != std::end(arguments);
		     it++) {
			stream << ", " << *(*it);
		}
	}

	stream << ((style == InvocationStyle::ParensAround) ? "))" : ")");
}
