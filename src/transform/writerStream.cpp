//
// Created by kmeinkopf on 15.01.2022.
//

#include "writerStream.hpp"

#include "transform/writers/writerBase.hpp"

kodgen::transform::WriterStream::WriterStream(std::ostream& targetStream)
	: oStream(targetStream) {}

void kodgen::transform::WriterStream::applyWriter(
	const kodgen::transform::WriterBase& writer) {
	writer.apply(oStream);
}

auto kodgen::transform::WriterStream::operator<<(
	const kodgen::transform::WriterBase& writer)
	-> kodgen::transform::WriterStream& {
	applyWriter(writer);

	return *this;
}
