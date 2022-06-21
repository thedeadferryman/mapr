//
// Created by karl on 22.03.22.
//

#include <filesystem>

#include "utils.hpp"

#include "util/stringBuilder.hpp"

#include "app/cli/commandLineException.hpp"

#include "config/configParser.hpp"

namespace FS = std::filesystem;

using mapr::config::ConfigParser;
using mapr::config::ToolProperties;

namespace mapr::app {

auto getConfigFile(std::string_view filePath, std::string_view entrypoint)
	-> ToolProperties {
	std::string file = FS::current_path() / filePath;

	if (!FS::exists(file)) {
		throw CommandLineException(
			std::string(util::StringBuilder("File not found: ") << file));
	}

	auto parser = ConfigParser(file, entrypoint);

	return parser.getConfig();
}

}  // namespace mapr::app