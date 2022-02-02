//
// Created by kmeinkopf on 26.01.2022.
//

#include "includeWriter.hpp"

#include "util/macro.hpp"

using kodgen::transform::IncludeWriter;

IncludeWriter::IncludeWriter(std::string_view file, IncludeType type)
	: file(file)
	, type(type) {}

void IncludeWriter::apply(std::ostream& stream) const {
	auto [argOpen, argClose] = getTypeArgEncloser(type);

	stream << "#include " << argOpen << file << argClose << "\n";
}

auto IncludeWriter::getTypeArgEncloser(IncludeType type)
	-> std::pair<std::string, std::string> {
	switch (type) {
		case IncludeType::System:
			return {"<", ">"};
		case IncludeType::Custom:
			return {"\"", "\""};
		default:
			UNREACHABLE();
	}
}
