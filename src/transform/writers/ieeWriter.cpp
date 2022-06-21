//
// Created by kmeinkopf on 27.01.2022.
//

#include "ieeWriter.hpp"

#include "util/collectionUtils.hpp"

using mapr::transform::IEEWriter;
using mapr::util::CollectionUtils;

IEEWriter::IEEWriter(std::unique_ptr<WriterBase> expr,
                     std::vector<std::unique_ptr<WriterBase>> args)
	: expr(std::move(expr))
	, args(std::move(args)) {}

void IEEWriter::apply(std::ostream& stream) const {
	stream << "((";
	expr->apply(stream);
	stream << ")(";

	if (!args.empty()) {
		(*args.begin())->apply(stream);
	}

	CollectionUtils::forEachSkipFirst(
		args, [&stream](const std::unique_ptr<WriterBase>& writer) {
			stream << ", ";
			writer->apply(stream);
		});

	stream << "))";
}
