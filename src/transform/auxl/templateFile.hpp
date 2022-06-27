//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include <filesystem>
#include <unordered_map>

#include "transform/aux/auxDecl.hpp"

#include "util/resourceLoader.hpp"

namespace mapr::transform {

class TemplateFile : public AuxDecl {
	std::shared_ptr<util::ResourceLoader> loader;

  public:
	TemplateFile(std::shared_ptr<util::ResourceLoader> loader,
	             std::string_view name);

	[[nodiscard]] auto render(
		const std::unordered_map<std::string, std::string>& variables) const
		-> std::string;

  private:
	[[nodiscard]] auto loadStreamContents() const -> std::string;
};

}  // namespace mapr::transform