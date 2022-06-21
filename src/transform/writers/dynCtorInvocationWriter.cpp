//
// Created by karl on 01.04.22.
//

#include "dynCtorInvocationWriter.hpp"

#include "transform/writers/textWriter.hpp"

using mapr::transform::DynCtorInvocationWriter;

DynCtorInvocationWriter::DynCtorInvocationWriter(
	std::unique_ptr<WriterBase> className,
	std::vector<std::unique_ptr<WriterBase>> args)
	: className(std::move(className))
	, args(std::move(args)) {}

void DynCtorInvocationWriter::apply(std::ostream& stream) const {
	stream << "(new " << className << "(";

	if (!args.empty()) {
		stream << args.front();

		for (auto it = std::begin(args) + 1; it != std::end(args); it++) {
			stream << ", " << (*it);
		}
	}
	stream << "))";
}
