//
// Created by kmeinkopf on 27.01.2022.
//

#include <sstream>

#include "lineEscapedWriter.hpp"

#include <boost/regex.hpp>

using kodgen::transform::LineEscapedWriter;

LineEscapedWriter::LineEscapedWriter(std::unique_ptr<WriterBase> subWriter)
	: subWriter(std::move(subWriter)) {}

void LineEscapedWriter::apply(std::ostream& stream) const {
	auto sstream = std::stringstream();

	subWriter->apply(sstream);

	auto rStr = boost::regex_replace(  //
		sstream.str(),  //
		boost::regex(R"(([\r\n]{1,2}))"),
		std::string(R"( \\$1)"));

	stream << rStr;
}
