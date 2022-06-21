//
// Created by karl on 01.04.22.
//

#include <utility>

#include "recordDtorDefinitionMapper.hpp"

#include "view/types/pointerType.hpp"

#include "transform/writers/assignmentWriter.hpp"
#include "transform/writers/cxxCastWriter.hpp"
#include "transform/writers/deleteWriter.hpp"
#include "transform/writers/dereferenceWriter.hpp"
#include "transform/writers/sequentialWriter.hpp"
#include "transform/writers/statement/plainStatementWriter.hpp"
#include "transform/writers/textWriter.hpp"

using mapr::transform::RecordDtorDefinitionMapper;

RecordDtorDefinitionMapper::RecordDtorDefinitionMapper(
	std::shared_ptr<const view::FunctionDecl> functionDecl,
	std::shared_ptr<config::PipelineContext> context)
	: FunctionLikeDefinitionMapper(std::move(functionDecl),
                                   std::move(context)) {}

void RecordDtorDefinitionMapper::writeOverloadBody(
	std::unique_ptr<BlockWriter>& block,
	const view::FunctionOverload& overload) {
	if (overload.getArguments().size() != 1) {
		UNREACHABLE_M("Can be only 1 arg for dtor!");
	}

	auto thisArg = overload.getArguments().front();
	auto thisArgType =
		std::dynamic_pointer_cast<view::PointerType>(thisArg->getType());

	if (!thisArgType) {
		UNREACHABLE_M("Dtor arg must be pointer!");
	}

	auto derefThis = std::make_unique<DereferenceWriter>(  //
		std::make_unique<TextWriter>(thisArg->getID())  //
	);

	auto derefThisType = std::make_unique<TextWriter>(
		thisArgType->getPointee()
			->getPrettyName());  // TODO: Replace prettyName here

	auto object = std::make_unique<CXXCastWriter>(  //
		std::move(derefThis),
		std::move(derefThisType),
		CastStyle::ReinterpretCast  //
	);

	derefThis = std::make_unique<DereferenceWriter>(  //
		std::make_unique<TextWriter>(thisArg->getID())  //
	);

	auto delStatement = std::make_unique<PlainStatementWriter>(  //
		std::make_unique<DeleteWriter>(std::move(object))  //
	);

	auto nullifyStatement = AssignmentWriter::statement(  //
		std::move(derefThis),
		std::make_unique<TextWriter>("nullptr")  //
	);

	(*block) << std::move(delStatement) << std::make_unique<TextWriter>("\n")
			 << std::move(nullifyStatement);
}
