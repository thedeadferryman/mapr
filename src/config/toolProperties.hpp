//
// Created by karl on 21.03.22.
//

#pragma once

#include <filesystem>
#include <optional>
#include <unordered_map>
#include <vector>

#include "filtering/filters/filterBase.hpp"

#define ALIGNMENT 128

namespace mapr::config {

struct FormatOptions {
	static const FormatOptions Defaults;

	const std::string formatStyle;
	const std::string fallbackStyle;
	const std::optional<std::filesystem::path> styleFile;
	const bool allowUnknownOptions;
} __attribute__((aligned(ALIGNMENT)));

struct ToolProperties {
	static const ToolProperties Defaults;

	const std::filesystem::path outPath;
	const std::string unitName;

	const std::optional<FormatOptions> formatOptions;
	const std::unordered_map<std::string, std::string> variables;

	const std::vector<std::shared_ptr<const FilterBase>> filters;
} __attribute__((aligned(ALIGNMENT)));

}  // namespace mapr::config

#undef ALIGNMENT