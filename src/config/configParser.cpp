//
// Created by karl on 21.03.22.
//

#include <regex>

#include "configParser.hpp"

#include <boost/algorithm/string.hpp>

#include "util/collectionUtils.hpp"

#include "filtering/filters/declNameFilter.hpp"
#include "filtering/filters/filenameFilter.hpp"

#include "config/invalidConfigException.hpp"

using mapr::config::ConfigParser;
using mapr::config::DeclNameFilter;
using mapr::config::FilenameFilter;
using mapr::config::FilterBase;
using mapr::config::FilterMode;
using mapr::config::FormatOptions;
using mapr::config::InvalidConfigException;

using mapr::util::CollectionUtils;

#define ALIGNMENT 128

struct FilterProps {
	std::string type;
	std::string matchMode;

	std::regex regex;

	FilterMode mode;
} __attribute__((aligned(ALIGNMENT))) __attribute__((packed));

#undef ALIGNMENT

[[nodiscard]] auto getFilterMode(const std::optional<std::string>& mode)
	-> std::optional<FilterMode>;

[[nodiscard]] auto createFilter(
	const FilterProps& props,
	mapr::config::InvalidConfigException::Location location)
	-> std::shared_ptr<FilterBase>;

ConfigParser::ConfigParser(std::string_view file, std::string_view entrypoint)
	: file(file)
	, entrypoint(entrypoint) {}

auto ConfigParser::getConfig() const -> ToolProperties {
	return parseConfig(YAML::LoadFile(file));
}

auto ConfigParser::parseFormatOptions(const std::optional<YAML::Node>& node)
	const -> std::optional<FormatOptions> {
	if (node == std::nullopt) {
		return FormatOptions::Defaults;
	}
	auto disabled = tryAs<bool>(node.value(), "disabled", /*fallback=*/false);

	if (disabled) {
		return std::nullopt;
	}

	return FormatOptions {
		.formatStyle = tryAs<std::string>(node.value(), "formatStyle")
						   .value_or(FormatOptions::Defaults.formatStyle),
		.fallbackStyle = tryAs<std::string>(node.value(), "fallbackStyle")
							 .value_or(FormatOptions::Defaults.fallbackStyle),
		.styleFile = tryAs<std::string>(node.value(), "styleFile"),
		.allowUnknownOptions =
			tryAs<bool>(node.value(),
	                    "allowUnknownOptions",
	                    FormatOptions::Defaults.allowUnknownOptions)
				.value_or(FormatOptions::Defaults.allowUnknownOptions)};
}

auto ConfigParser::parseVariables(std::optional<YAML::Node> node) const
	-> std::unordered_map<std::string, std::string> {
	auto variables = std::unordered_map<std::string, std::string>();

	variables.insert(  //
		std::begin(ToolProperties::Defaults.variables),
		std::end(ToolProperties::Defaults.variables)  //
	);

	if (node == std::nullopt) {
		return variables;
	}

	for (const auto& sub : node.value()) {
		auto name = sub.first.as<std::string>();
		auto value = sub.second;

		if (value.IsNull()) {
			continue;
		}

		if (!value.IsScalar()) {
			throw InvalidConfigException("expected scalar",  //
			                             {
											 .file = file,
											 .line = value.Mark().line,
											 .column = value.Mark().column,
											 .property = name,
										 });
		}

		variables[name] = value.as<std::string>();
	}

	return variables;
}

auto ConfigParser::parseConfig(const YAML::Node& rawConfig) const
	-> ToolProperties {
	auto excludeEntrypoint =
		tryAs<bool>(rawConfig, "excludeEntrypoint").value_or(false);

	return {
		.outPath = tryAs<std::string>(rawConfig, "output")
					   .value_or(ToolProperties::Defaults.outPath),
		.unitName = tryAs<std::string>(rawConfig, "unitName")
						.value_or(ToolProperties::Defaults.unitName),
		.formatOptions =
			parseFormatOptions(tryObject(rawConfig, "formatOptions")),
		.variables = parseVariables(tryObject(rawConfig, "variables")),
		.filters =
			parseFilters(tryArray(rawConfig, "filters"), excludeEntrypoint),
	};
}

auto ConfigParser::tryScalar(const YAML::Node& base,
                             std::string_view name) const
	-> std::optional<YAML::Node> {
	auto node = base[std::string(name)];

	if (!node.IsDefined() || node.IsNull()) {
		return std::nullopt;
	}

	if (!node.IsScalar()) {
		throw InvalidConfigException(  //
			"expected scalar",
			{
				.file = file.string(),
				.line = node.Mark().line,
				.column = node.Mark().column,
				.property = std::string(name),
			});
	}

	return node;
}
auto ConfigParser::tryObject(const YAML::Node& base,
                             std::string_view name) const
	-> std::optional<YAML::Node> {
	auto node = base[std::string(name)];

	if (!node.IsDefined() || node.IsNull()) {
		return std::nullopt;
	}

	if (!node.IsMap()) {
		throw InvalidConfigException("expected object",
		                             {
										 .file = file.string(),
										 .line = node.Mark().line,
										 .column = node.Mark().column,
										 .property = std::string(name),
									 });
	}

	return node;
}

auto ConfigParser::parseFilters(std::optional<YAML::Node> node,
                                bool excludeEntrypoint) const
	-> std::vector<std::shared_ptr<const FilterBase>> {
	auto filters = std::vector<std::shared_ptr<const FilterBase>>();

	if (!excludeEntrypoint) {
		filters.push_back(std::make_shared<FilenameFilter>(  //
			FilterMode::Include,
			FilenameFilter::Mode::RelativePath,
			std::regex(entrypoint.string())  //
			));
	}

	if (!node) {
		return filters;
	}

	for (auto filterNode : node.value()) {
		if (filterNode.Type() != YAML::NodeType::Map) {
			throw InvalidConfigException("expected object",
			                             {
											 .file = file.string(),
											 .line = filterNode.Mark().line,
											 .column = filterNode.Mark().column,
											 .property = "filters",
										 });
		}

		if (auto filter = parseFilter(filterNode)) {
			filters.emplace_back(std::move(filter));
		}
	}

	return filters;
}

auto ConfigParser::tryArray(const YAML::Node& base, std::string_view name) const
	-> std::optional<YAML::Node> {
	auto node = base[std::string(name)];

	if (!node.IsDefined() || node.IsNull()) {
		return std::nullopt;
	}

	if (!node.IsSequence()) {
		throw InvalidConfigException("expected array",
		                             {
										 .file = file.string(),
										 .line = node.Mark().line,
										 .column = node.Mark().column,
										 .property = std::string(name),
									 });
	}

	return node;
}

auto ConfigParser::parseFilter(const YAML::Node& node) const
	-> std::shared_ptr<FilterBase> {
	auto maybeType = tryAs<std::string>(node, "by");

	if (!maybeType) {
		throw InvalidConfigException("filter type not specified",
		                             {
										 .file = file.string(),
										 .line = node.Mark().line,
										 .column = node.Mark().column,
										 .property = "by",
									 });
	}

	auto parts = std::vector<std::string>();

	boost::split(parts, maybeType.value(), boost::is_any_of("#"));

	if (parts.size() != 2) {
		throw InvalidConfigException("invalid filter type",
		                             {
										 .file = file.string(),
										 .line = node.Mark().line,
										 .column = node.Mark().column,
										 .property = "by",
									 });
	}

	auto [type, matchMode] = CollectionUtils::decompose<2>(parts);

	auto maybeMode = getFilterMode(tryAs<std::string>(node, "mode"));

	if (!maybeMode) {
		throw InvalidConfigException("invalid filter mode",
		                             {
										 .file = file.string(),
										 .line = node.Mark().line,
										 .column = node.Mark().column,
										 .property = "mode",
									 });
	}

	auto regexSource = tryAs<std::string>(node, "regex").value_or(".*");
	auto regex = std::regex(regexSource);

	return createFilter(
		{
			.type = type,
			.matchMode = matchMode,
			.regex = regex,
			.mode = maybeMode.value(),
		},
		{
			.file = file.string(),
			.line = node.Mark().line,
			.column = node.Mark().column,
			.property = std::nullopt,
		});
}

auto getFilterMode(const std::optional<std::string>& mode)
	-> std::optional<FilterMode> {
	if (mode == "include") {
		return FilterMode::Include;
	}
	if (mode == "exclude") {
		return FilterMode::Exclude;
	}

	return std::nullopt;
}

template<class Filter>
	requires(std::derived_from<Filter, FilterBase>)
inline auto createTypedFilter(const FilterProps& props,
                              InvalidConfigException::Location location) {
	auto maybeMode = Filter::matchModeFromString(props.matchMode);

	if (!maybeMode) {
		location.property = "by";
		throw InvalidConfigException("invalid match mode", location);
	}

	return std::make_shared<Filter>(props.mode, maybeMode.value(), props.regex);
}

auto createFilter(const FilterProps& props,
                  InvalidConfigException::Location location)
	-> std::shared_ptr<FilterBase> {
	if (props.type == "decl") {
		return createTypedFilter<DeclNameFilter>(props, location);
	}
	if (props.type == "file") {
		return createTypedFilter<FilenameFilter>(props, location);
	}

	location.property = "by";
	throw InvalidConfigException("invalid filter type", location);
}
