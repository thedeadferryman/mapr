//
// Created by kmeinkopf on 19.01.2022.
//

#include "externCWriter.hpp"

#include "transform/writers/macroBlockWriter.hpp"

#include "util/macro.hpp"

#include "transform/writers/textWriter.hpp"

using mapr::transform::ExternCWriter;

ExternCWriter::ExternCWriter(std::shared_ptr<config::PipelineContext> context)
	: context(std::move(context)) {}

void ExternCWriter::apply(std::ostream& stream) const {
	stream << getExportSpecifier(context) << " ";
}

auto ExternCWriter::makeBlock(
	const std::shared_ptr<config::PipelineContext>& context)
	-> MacroBlockWriter {
	return {
		std::make_unique<TextWriter>(getExportSpecifier(context) + "_OPEN"),
		std::make_unique<TextWriter>(getExportSpecifier(context) + "_CLOSE")  //
	};
}

auto ExternCWriter::getExportSpecifier(
	const std::shared_ptr<config::PipelineContext>& context) -> std::string {
	auto maybeSpecifier = context->readConfigVariable("exportSpecifier");

	if (maybeSpecifier.has_value()) {
		return maybeSpecifier.value();
	}
	UNREACHABLE();
}
