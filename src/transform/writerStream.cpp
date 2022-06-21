//
// Created by kmeinkopf on 15.01.2022.
//

#include "writerStream.hpp"

#include "transform/writers/writerBase.hpp"

mapr::transform::WriterStream::WriterStream(std::ostream& targetStream)
	: oStream(targetStream) {}

void mapr::transform::WriterStream::applyWriter(
	const mapr::transform::WriterBase& writer) {
	writer.apply(oStream);
}

auto mapr::transform::WriterStream::operator<<(
	const mapr::transform::WriterBase& writer)
	-> mapr::transform::WriterStream& {
	applyWriter(writer);

	return *this;
}
