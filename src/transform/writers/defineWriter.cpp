//
// Created by kmeinkopf on 27.01.2022.
//

#include "defineWriter.hpp"

#include "transform/writers/lineEscapedWriter.hpp"

#include "util/collectionUtils.hpp"

using mapr::transform::DefineWriter;
using mapr::util::CollectionUtils;

DefineWriter::DefineWriter(
	std::unique_ptr<WriterBase> idWriter,
	std::optional<std::vector<std::unique_ptr<WriterBase>>> argWriters,
	std::unique_ptr<WriterBase> bodyWriter)
	: idWriter(std::move(idWriter))
	, argWriters(std::move(argWriters))
	, bodyWriter(std::make_unique<LineEscapedWriter>(std::move(bodyWriter))) {}

void DefineWriter::apply(std::ostream& stream) const {
	stream << "#define ";
	idWriter->apply(stream);

	if (argWriters.has_value()) {
		stream << "(";

		if (!argWriters->empty()) {
			(*argWriters->begin())->apply(stream);
		}

		CollectionUtils::forEachSkipFirst(
			argWriters.value(),
			[&stream](const std::unique_ptr<WriterBase>& writer) {
				stream << ", ";
				writer->apply(stream);
			});

		stream << ")";
	}

	stream << " ";

	bodyWriter->apply(stream);

	stream << "\n";
}
