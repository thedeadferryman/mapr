//
// Created by karl on 22.03.22.
//

#include <filesystem>

#include "pipelineContext.hpp"

#include "util/stringBuilder.hpp"

using mapr::config::PipelineContext;
using mapr::util::StringBuilder;
namespace FS = std::filesystem;

PipelineContext::PipelineContext(ToolProperties config,
                                 std::string_view inputFile)
	: config(std::move(config))
	, inputFile(inputFile) {}

auto PipelineContext::getHeaderName() const -> std::string {
	return StringBuilder(config.unitName) << ".h";
}

auto PipelineContext::getImplName() const -> std::string {
	return StringBuilder(config.unitName) << ".cpp";
}

auto PipelineContext::getInputFile() const -> std::string {
	return inputFile;
}

auto PipelineContext::getInputFileRelative() const -> std::string {
	auto outDir = FS::current_path() / config.outPath;

	return FS::proximate(FS::current_path() / inputFile, outDir);
}

auto PipelineContext::getConfig() const -> const ToolProperties& {
	return config;
}
auto PipelineContext::readConfigVariable(std::string_view name) const
	-> std::optional<std::string> {
	if (!config.variables.contains(std::string(name))) {
		return std::nullopt;
	}

	return config.variables.at(std::string(name));
}
