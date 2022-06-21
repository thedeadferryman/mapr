//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include "util/macro.hpp"

namespace mapr::util {

S_ENUM(ResourceCategory, TemplateFile);

class ResourceLoader {
	std::filesystem::path resourcesDir;

  public:
	ResourceLoader(const std::filesystem::path& resourcesDir);

	ResourceLoader(std::string_view resourcesDir);

	[[nodiscard]] auto loadResource(ResourceCategory category,
	                                std::string_view name) const
		-> std::unique_ptr<std::ifstream>;
};

}  // namespace mapr::util