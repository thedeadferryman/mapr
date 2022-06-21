//
// Created by karl on 11.04.22.
//

#include <utility>

#include "syntheticSetterDefinitionMapper.hpp"

#include "transform/writers/assignmentWriter.hpp"
#include "transform/writers/memberAccessWriter.hpp"
#include "transform/writers/statement/returnStatementWriter.hpp"
#include "transform/writers/textWriter.hpp"

using mapr::transform::SyntheticSetterDefinitionMapper;

SyntheticSetterDefinitionMapper::SyntheticSetterDefinitionMapper(
	std::shared_ptr<const view::FunctionDecl> decl,
	std::shared_ptr<config::PipelineContext> context)
	: FunctionLikeDefinitionMapper(std::move(decl), std::move(context)) {}

void SyntheticSetterDefinitionMapper::writeOverloadBody(
	std::unique_ptr<BlockWriter>& block,
	const view::FunctionOverload& overload) {
	if (overload.countArguments() != 2) {
		UNREACHABLE_M("Setter with additional arguments!");
	}

	auto thisArg = overload.getArguments().front();
	auto valueArg = overload.getArguments().back();

	auto fieldAccess = std::make_unique<MemberAccessWriter>(
		writeArgument(thisArg),
		std::make_unique<TextWriter>(getDecl()->getQualifiedName().str()),
		MemberAccessMode::PointerLike);

	(*block) << AssignmentWriter::statement(std::move(fieldAccess),
	                                        writeArgument(valueArg));
}
