//
// Created by kmeinkopf on 20.01.2022.
//

#include "textWriter.hpp"

using kodgen::transform::TextWriter;

TextWriter::TextWriter(std::string_view content)
	: content(content) {}

void TextWriter::apply(std::ostream& stream) const {
	stream << content;
}
