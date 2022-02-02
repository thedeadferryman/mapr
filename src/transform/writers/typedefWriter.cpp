//
// Created by kmeinkopf on 02.02.2022.
//

#include "typedefWriter.hpp"

using kodgen::transform::TypedefWriter;

TypedefWriter::TypedefWriter(std::unique_ptr<WriterBase> origin,
                             std::unique_ptr<WriterBase> alias)
	: origin(std::move(origin))
	, alias(std::move(alias)) {}

void TypedefWriter::apply(std::ostream& stream) const {
	stream << "typedef ";

	origin->apply(stream);
	stream << " ";
	alias->apply(stream);

	stream << ";";
}
