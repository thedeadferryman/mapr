//
// Created by karl on 11.04.22.
//

#include <utility>

#include "syntheticGetterDefinitionMapper.hpp"

#include "transform/writers/memberAccessWriter.hpp"
#include "transform/writers/statement/returnStatementWriter.hpp"
#include "transform/writers/textWriter.hpp"

using mapr::transform::SyntheticGetterDefinitionMapper;

SyntheticGetterDefinitionMapper::SyntheticGetterDefinitionMapper(
	std::shared_ptr<const view::FunctionDecl> decl,
	std::shared_ptr<config::PipelineContext> context)
	: FunctionLikeDefinitionMapper(std::move(decl), std::move(context)) {}

void SyntheticGetterDefinitionMapper::writeOverloadBody(
	std::unique_ptr<BlockWriter>& block,
	const view::FunctionOverload& overload) {
	if (overload.countArguments() > 1) {
		UNREACHABLE_M("Getter with arguments!");
	}

	auto thisArg = overload.getArguments().front();

	auto fieldAccess = std::make_unique<MemberAccessWriter>(
		writeArgument(thisArg),
		std::make_unique<TextWriter>(getDecl()->getQualifiedName().str()),
		MemberAccessMode::PointerLike);

	(*block) << std::make_unique<ReturnStatementWriter>(
		writeReturnValue(overload.getReturnType(), std::move(fieldAccess)));
}
