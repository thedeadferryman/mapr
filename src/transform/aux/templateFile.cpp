//
// Created by kmeinkopf on 19.01.2022.
//

#include <sstream>

#include "templateFile.hpp"

#include <fmt/format.h>

using kodgen::transform::TemplateFile;
using kodgen::util::ResourceCategory;
using kodgen::util::ResourceLoader;

TemplateFile::TemplateFile(const ResourceLoader& loader, std::string_view name)
	: AuxDecl(name)
	, templateStream(loader.loadResource(ResourceCategory::TemplateFile, name))
	, isLoaded(false) {}

auto TemplateFile::replaceWith() -> std::string {
	return fmt::vformat(loadStreamContents(),
	                    fmt::make_format_args(
							fmt::arg("overloadPreludeDefinePrefix",
	                                                   "KODGEN_PRELUDE_"),
							fmt::arg("externDefinePrefix", "KODGEN_EXPORT")
							));
}
auto kodgen::transform::TemplateFile::loadStreamContents() -> std::string_view {
	if (isLoaded) {
		return templateString;
	}

	std::stringstream accum;
	std::string buf;

	while (!templateStream->eof()) {
		std::getline(*templateStream, buf);

		accum << buf << std::endl;

		buf.clear();
	}

	return templateString = accum.str();
}
