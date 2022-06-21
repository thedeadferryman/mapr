//
// Created by karl on 21.03.22.
//

#pragma once

#include <filesystem>
#include <string>

#include "config/toolProperties.hpp"

#include <yaml-cpp/yaml.h>

namespace mapr::config {

class ConfigParser {
	std::filesystem::path file;
	std::filesystem::path entrypoint;

  public:
	explicit ConfigParser(std::string_view file, std::string_view entrypoint);

	[[nodiscard]] auto getConfig() const -> ToolProperties;

  private:
	[[nodiscard]] auto parseConfig(const YAML::Node& rawConfig) const
		-> ToolProperties;

	[[nodiscard]] auto parseFilters(std::optional<YAML::Node> node,
	                                bool excludeEntrypoint) const
		-> std::vector<std::shared_ptr<const FilterBase>>;

	[[nodiscard]] auto parseFormatOptions(const std::optional<YAML::Node>& node)
		const -> std::optional<FormatOptions>;

	[[nodiscard]] auto parseVariables(std::optional<YAML::Node> node) const
		-> std::unordered_map<std::string, std::string>;

	[[nodiscard]] auto tryScalar(const YAML::Node& base,
	                             std::string_view name) const
		-> std::optional<YAML::Node>;

	[[nodiscard]] auto tryArray(const YAML::Node& node,
	                            std::string_view name) const
		-> std::optional<YAML::Node>;

	[[nodiscard]] auto tryObject(const YAML::Node& base,
	                             std::string_view name) const
		-> std::optional<YAML::Node>;

	[[nodiscard]] auto parseFilter(const YAML::Node& node) const
		-> std::shared_ptr<FilterBase>;

	template<typename T>
	[[nodiscard]] auto tryAs(const YAML::Node& base,
	                         std::string_view name) const -> std::optional<T> {
		auto node = tryScalar(base, name);

		if (node == std::nullopt) {
			return std::nullopt;
		}

		return node->as<T>();
	}

	template<typename T>
	[[nodiscard]] auto tryAs(const YAML::Node& base,
	                         std::string_view name,
	                         T fallback) const -> std::optional<T> {
		auto node = tryScalar(base, name);

		if (node == std::nullopt) {
			return std::nullopt;
		}

		return node->as<T>(fallback);
	}
};

}  // namespace mapr::config
