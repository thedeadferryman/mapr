//
// Created by kmeinkopf on 19.01.2022.
//

#include "functionHeadWriter.hpp"

#include <boost/algorithm/string/join.hpp>

#include "transform/writers/externCWriter.hpp"

using mapr::transform::FunctionHeadWriter;

FunctionHeadWriter::FunctionHeadWriter(
	std::shared_ptr<config::PipelineContext> context,
	std::unique_ptr<WriterBase> name,
	std::unique_ptr<WriterBase> returnType,
	std::vector<Argument> arguments,
	Mode mode)
	: mode(mode)
	, name(std::move(name))
	, returnType(std::move(returnType))
	, arguments(std::move(arguments))
	, context(std::move(context)) {}

void FunctionHeadWriter::apply(std::ostream& stream) const {
	if (mode != Mode::SplitDefinition) {
		ExternCWriter(context).apply(stream);
	}

	stream << *returnType << ' ' << *name << "(";

	auto argStrings = std::vector<std::string>();

	std::transform(std::begin(arguments),
	               std::end(arguments),
	               std::back_inserter(argStrings),
	               [](const Argument& entry) -> std::string {
					   return entry.type + " " + entry.name;
				   });

	stream << boost::algorithm::join(argStrings, ", ") << ")";

	if (mode == Mode::Declaration) {
		stream << ";\n";
	}
}