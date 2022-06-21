//
// Created by kmeinkopf on 01.03.22.
//

#include "expressionCallWriter.hpp"

#include "util/collectionUtils.hpp"

using mapr::transform::ExpressionCallWriter;
using mapr::util::CollectionUtils;

ExpressionCallWriter::ExpressionCallWriter(
	std::unique_ptr<WriterBase> expr,
	std::vector<std::unique_ptr<WriterBase>> args)
	: WriterBase()
	, expr(std::move(expr))
	, args(std::move(args)) {}

void ExpressionCallWriter::apply(std::ostream& stream) const {
	expr->apply(stream);
	stream << "(";

	if (!args.empty()) {
		(*args.begin())->apply(stream);
	}

	CollectionUtils::forEachSkipFirst(
		args, [&stream](const std::unique_ptr<WriterBase>& writer) {
			stream << ", ";
			writer->apply(stream);
		});

	stream << ")";
}
