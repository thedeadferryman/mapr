//
// Created by kmeinkopf on 26.01.2022.
//

#include "c11GenericWriter.hpp"

#include "util/stringBuilder.hpp"

using mapr::transform::C11GenericWriter;

C11GenericWriter::C11GenericWriter(std::unique_ptr<WriterBase> defaultCase,
                                   std::unique_ptr<WriterBase> condition,
                                   std::vector<Case> cases)
	: cases(std::move(cases))
	, defaultCase(std::move(defaultCase))
	, condition(std::move(condition)) {}

void C11GenericWriter::apply(std::ostream& stream) const {
	stream << "_Generic(";
	condition->apply(stream);

	for (const auto& kase : cases) {
		stream << ",\n\t";
		kase.type->apply(stream);
		stream << " : ";
		kase.value->apply(stream);
	}

	if (defaultCase) {
		stream << ",\n\tdefault : ";
		defaultCase->apply(stream);
	}
	stream << ")";
}
