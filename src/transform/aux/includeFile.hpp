//
// Created by karl on 22.03.22.
//

#pragma once

#include "transform/aux/auxDecl.hpp"
#include "transform/writers/includeWriter.hpp"

namespace mapr::transform {

class IncludeFile : public AuxDecl {
	std::string filename;
	IncludeMode mode;

  public:
	explicit IncludeFile(std::string_view filename, IncludeMode mode = IncludeMode::Custom);

	[[nodiscard]] auto getFilename() const -> std::string;

	[[nodiscard]] auto getMode() const -> IncludeMode;
};

}  // namespace mapr::transform


