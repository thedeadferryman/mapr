//
// Created by kmeinkopf on 02.02.2022.
//

#include "macroBlockWriter.hpp"

#include "transform/writers/textWriter.hpp"

using mapr::transform::MacroBlockWriter;

MacroBlockWriter::MacroBlockWriter(std::unique_ptr<WriterBase> opener,
                                   std::unique_ptr<WriterBase> closer)
	: opener(std::move(opener))
	, closer(std::move(closer)) {}

MacroBlockWriter::MacroBlockWriter(
	std::unique_ptr<WriterBase> opener,
	std::unique_ptr<WriterBase> closer,
	std::vector<std::unique_ptr<WriterBase>> body)
	: opener(std::move(opener))
	, closer(std::move(closer))
	, body(std::move(body)) {}

void MacroBlockWriter::apply(std::ostream& stream) const {
	stream << opener << TextWriter::Newline;

	for (const auto& bodyWriter : body) {
		stream << bodyWriter;
	}

	stream << TextWriter::Newline << closer;
}

auto MacroBlockWriter::operator<<(std::unique_ptr<WriterBase> writer)
	-> MacroBlockWriter& {
	body.emplace_back(std::move(writer));

	return *this;
}
