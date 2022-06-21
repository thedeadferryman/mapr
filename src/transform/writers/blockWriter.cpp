//
// Created by kmeinkopf on 02.02.2022.
//

#include "blockWriter.hpp"

using mapr::transform::BlockWriter;

BlockWriter::BlockWriter(std::unique_ptr<WriterBase> prelude, bool semicolon)
	: prelude(std::move(prelude))
	, semicolon(semicolon) {}

BlockWriter::BlockWriter(std::unique_ptr<WriterBase> prelude,
                         std::vector<std::unique_ptr<WriterBase>> body,
                         bool semicolon)
	: prelude(std::move(prelude))
	, body(std::move(body))
	, semicolon(semicolon) {}

void BlockWriter::apply(std::ostream& stream) const {
	prelude->apply(stream);

	stream << "{\n";

	for (const auto& bodyWriter : body) {
		bodyWriter->apply(stream);
	}

	stream << "\n}";

	if (semicolon) {
		stream << ";";
	}
}

auto BlockWriter::operator<<(std::unique_ptr<WriterBase> writer)
	-> BlockWriter& {
	body.emplace_back(std::move(writer));

	return *this;
}
