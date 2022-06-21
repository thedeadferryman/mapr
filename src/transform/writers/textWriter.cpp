//
// Created by kmeinkopf on 20.01.2022.
//

#include "textWriter.hpp"

using mapr::transform::TextWriter;

TextWriter const TextWriter::Newline = TextWriter("\n");

TextWriter::TextWriter(std::string_view content) noexcept
	: content(content) {}

void TextWriter::apply(std::ostream& stream) const {
	stream << content;
}
