//
// Created by kmeinkopf on 19.01.2022.
//

#include "externCWriter.hpp"

#include "transform/writers/blockWriter.hpp"

#include "util/macro.hpp"

using kodgen::transform::ExternCWriter;

const ExternCWriter ExternCWriter::Inline =
	ExternCWriter(ExternCWriter::ExternKind::Inline);

const ExternCWriter ExternCWriter::OpenBlock =
	ExternCWriter(ExternCWriter::ExternKind::OpenBlock);
const ExternCWriter ExternCWriter::CloseBlock =
	ExternCWriter(ExternCWriter::ExternKind::CloseBlock);

ExternCWriter::ExternCWriter(ExternCWriter::ExternKind kind) noexcept
	: kind(kind) {}

void ExternCWriter::apply(std::ostream& stream) const {
	switch (kind) {  // TODO allow configuring these defines
		case ExternKind::Inline:
			stream << "KODGEN_EXPORT ";
			return;
		case ExternKind::OpenBlock:
			stream << "KODGEN_EXPORT_BLOCK_START\n";
			return;
		case ExternKind::CloseBlock:
			stream << "KODGEN_EXPORT_BLOCK_END\n";
			return;
		default:
			UNREACHABLE();
	}
}

auto ExternCWriter::makeBlock() -> kodgen::transform::BlockWriter {
	return BlockWriter(
		std::unique_ptr<ExternCWriter>(new ExternCWriter(ExternKind::Inline)));
}
