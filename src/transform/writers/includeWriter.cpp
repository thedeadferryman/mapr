//
// Created by kmeinkopf on 26.01.2022.
//

#include "includeWriter.hpp"

#include "util/macro.hpp"

using mapr::transform::IncludeWriter;

IncludeWriter::IncludeWriter(std::string_view file, IncludeMode type)
	: file(file)
	, type(type) {}

void IncludeWriter::apply(std::ostream& stream) const {
	auto [argOpen, argClose] = getTypeArgEncloser(type);

	stream << "#include " << argOpen << file << argClose << "\n";
}

auto IncludeWriter::getTypeArgEncloser(IncludeMode type)
	-> std::pair<std::string, std::string> {
	switch (type) {
		case IncludeMode::System:
			return {"<", ">"};
		case IncludeMode::Custom:
			return {"\"", "\""};
		default:
			UNREACHABLE();
	}
}
