//
// Created by kmeinkopf on 19.01.2022.
//

#include <sstream>

#include "templateFile.hpp"

#include <fmt/args.h>
#include <fmt/format.h>

using mapr::transform::TemplateFile;
using mapr::util::ResourceCategory;
using mapr::util::ResourceLoader;

auto buildFormatArgStore(
	const std::unordered_map<std::string, std::string>& variables) {
	fmt::dynamic_format_arg_store<fmt::format_context> store;

	for (const auto& [key, value] : variables) {
		store.push_back(fmt::arg(key.c_str(), value));
	}

	return store;
}

TemplateFile::TemplateFile(std::shared_ptr<ResourceLoader> loader,
                           std::string_view name)
	: AuxDecl(name)
	, loader(std::move(loader)) {}

auto TemplateFile::render(
	const std::unordered_map<std::string, std::string>& variables) const
	-> std::string {
	return fmt::vformat(loadStreamContents(), buildFormatArgStore(variables));
}
auto TemplateFile::loadStreamContents() const -> std::string {
	std::unique_ptr<std::ifstream> templateStream = loader->loadResource(  //
		util::ResourceCategory::TemplateFile,
		getID()  //
	);

	std::stringstream accum;
	std::string buf;

	while (!templateStream->eof()) {
		std::getline(*templateStream, buf);

		accum << buf << std::endl;

		buf.clear();
	}

	return accum.str();
}
