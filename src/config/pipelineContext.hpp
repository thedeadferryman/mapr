//
// Created by karl on 22.03.22.
//

#pragma once

#include "config/toolProperties.hpp"

namespace mapr::config {

class PipelineContext {
	ToolProperties config;
	std::string inputFile;

  public:
	PipelineContext(ToolProperties config, std::string_view inputFile);

	auto getHeaderName() const -> std::string;

	auto getImplName() const -> std::string;

	auto getInputFile() const -> std::string;

	auto getInputFileRelative() const -> std::string;

	auto readConfigVariable(std::string_view name) const -> std::optional<std::string>;

	auto getConfig() const -> const ToolProperties&;
};

}  // namespace mapr::config


