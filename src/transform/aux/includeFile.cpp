//
// Created by karl on 22.03.22.
//

#include "includeFile.hpp"

using mapr::transform::IncludeFile;

IncludeFile::IncludeFile(std::string_view filename, IncludeMode mode)
	: AuxDecl(std::string("#include") + std::string(filename))
	, filename(filename)
	, mode(mode) {}

auto IncludeFile::getFilename() const -> std::string {
	return filename;
}
auto IncludeFile::getMode() const -> IncludeMode {
	return mode;
}
