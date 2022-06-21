//
// Created by karl on 22.03.22.
//

#pragma once

#include <clang/Tooling/CommonOptionsParser.h>

#include "config/toolProperties.hpp"

namespace mapr::app {

[[nodiscard]] auto getConfigFile(std::string_view filePath,
                                 std::string_view entrypoint) -> config::ToolProperties;

}  // namespace mapr::app
