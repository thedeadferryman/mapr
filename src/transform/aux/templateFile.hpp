//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include <filesystem>

#include "transform/aux/auxDecl.hpp"

#include "util/resourceLoader.hpp"

namespace kodgen::transform {

class TemplateFile : public AuxDecl {
  private:
	std::unique_ptr<std::istream> templateStream;
	std::string templateString;
	bool isLoaded;

	static constexpr auto BUF_SIZE = 256;

  public:
	explicit TemplateFile(const util::ResourceLoader& loader,
	                      std::string_view name);

	[[nodiscard]] auto replaceWith(/* TODO implement config context */)
		-> std::string;

  private:
	[[nodiscard]] auto loadStreamContents() -> std::string_view;
};

}  // namespace kodgen::getName