//
// Created by kmeinkopf on 02.02.2022.
//

#include "sequentialWriter.hpp"

#include "util/collectionUtils.hpp"

using mapr::transform::SequentialWriter;
using mapr::util::CollectionUtils;

auto SequentialWriter::joinToPtr(
	std::vector<std::unique_ptr<WriterBase>> writers,
	std::function<std::unique_ptr<WriterBase>()> glue)
	-> std::unique_ptr<SequentialWriter> {
	auto newWriters = std::vector<std::unique_ptr<WriterBase>>();

	newWriters.emplace_back(std::move(writers.at(0)));

	for (auto writer = std::begin(writers); writer != std::end(writers);
	     writer++) {
		newWriters.push_back(glue());
		newWriters.emplace_back(std::move(*writer));
	}

	return std::make_unique<SequentialWriter>(std::move(newWriters));
}

SequentialWriter::SequentialWriter(
	std::vector<std::unique_ptr<WriterBase>> writers)
	: writers(std::move(writers)) {}

void SequentialWriter::apply(std::ostream& stream) const {
	for (const auto& writer : writers) {
		writer->apply(stream);
	}
}
