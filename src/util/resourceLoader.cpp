//
// Created by kmeinkopf on 19.01.2022.
//

#include <filesystem>
#include <fstream>
#include <optional>

#include "resourceLoader.hpp"

#include "util/exceptions/resourceNotFoundException.hpp"

using kodgen::util::ResourceCategory;
using kodgen::util::ResourceLoader;

namespace FS = std::filesystem;

constexpr auto mapCategorySubdir(ResourceCategory category)
	-> std::string_view {
	switch (category) {
		case ResourceCategory::TemplateFile:
			return "templates";
	}
}

constexpr auto mapCategoryFileMode(ResourceCategory category)
	-> std::ios::openmode {
	switch (category) {
		case ResourceCategory::TemplateFile:
			return std::ios::in;
	}
}

ResourceLoader::ResourceLoader(const std::filesystem::path& resourcesDir)
	: resourcesDir(resourcesDir) {}

ResourceLoader::ResourceLoader(std::string_view resourcesDir)
	: resourcesDir(resourcesDir) {}

auto ResourceLoader::loadResource(ResourceCategory category,
                                  std::string_view name) const
	-> std::unique_ptr<std::ifstream> {
	auto fullPath = resourcesDir / mapCategorySubdir(category) / name;

	if (!FS::exists(fullPath)) {
		throw ResourceNotFoundException(category, name);
	}

	return std::make_unique<std::ifstream>(fullPath,
	                                       mapCategoryFileMode(category));
}
