//
// Created by kmeinkopf on 19.01.2022.
//

#include "functionWriter.hpp"

#include <boost/algorithm/string/join.hpp>

#include "transform/writers/externCWriter.hpp"

using kodgen::transform::FunctionWriter;

FunctionWriter::FunctionWriter(std::string_view name,
                               std::string_view returnType,
                               std::vector<Argument> arguments)
	: name(name)
	, returnType(returnType)
	, arguments(std::move(arguments)) {}

void FunctionWriter::apply(std::ostream& stream) const {
	ExternCWriter::Inline.apply(stream);
	stream << returnType << ' ' << name << "(";

	auto argStrings = std::vector<std::string>();

	std::transform(std::begin(arguments),
	               std::end(arguments),
	               std::back_inserter(argStrings),
	               [](const Argument& entry) -> std::string {
					   return entry.type + " " + entry.name;
				   });

	stream << boost::algorithm::join(argStrings, ", ") << ");\n";
}