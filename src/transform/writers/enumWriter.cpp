//
// Created by kmeinkopf on 02.02.2022.
//

#include <algorithm>

#include "enumWriter.hpp"

#include "transform/writers/blockWriter.hpp"
#include "transform/writers/sequentialWriter.hpp"
#include "transform/writers/textWriter.hpp"

using kodgen::transform::EnumWriter;

EnumWriter::EnumWriter(std::unique_ptr<WriterBase> name)
	: name(std::move(name)) {}

void EnumWriter::apply(std::ostream& stream) const {
	stream << "enum ";
	name->apply(stream);

	stream << " { ";

	(*members.begin())->apply(stream);

	for (auto member = (members.begin() + 1);  //
	     member != members.end();
	     member++) {
		stream << ", ";
		(*member)->apply(stream);
	}

	stream << " }";
}

auto EnumWriter::operator<<(std::unique_ptr<WriterBase> member) -> EnumWriter& {
	members.emplace_back(std::move(member));

	return *this;
}
