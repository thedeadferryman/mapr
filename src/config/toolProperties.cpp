//
// Created by karl on 21.03.22.
//

#include <filesystem>
#include <optional>

#include "toolProperties.hpp"

using mapr::config::FormatOptions;
using mapr::config::ToolProperties;

const FormatOptions FormatOptions::Defaults = {
	.formatStyle = "LLVM",
	.fallbackStyle = "LLVM",
	.styleFile = std::nullopt,
	.allowUnknownOptions = false,
};

const ToolProperties ToolProperties::Defaults = {
	.outPath = std::filesystem::current_path() / "out",
	.unitName = "mappings",
	.formatOptions = FormatOptions::Defaults,
	.variables = {{"preludePrefix", "MAPR_PRELUDE"},
                  {"exportSpecifier", "MAPR_EXPORT"},
                  {"namePrefix", "EXTERN_"},
                  {"constructorName", "init"},
                  {"destructorName", "dispose"},
                  {"getterPattern", "get_{}"},
                  {"setterPattern", "set_{}"},
                  {"operatorPattern", "op_{}"},
                  {"classTypeSuffix", "_CT"}},
	.filters = {},
};
