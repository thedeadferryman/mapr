//
// Created by karl on 31.03.22.
//

#include "recordMethodDefinitionMapper.hpp"

#include "transform/writers/function/functionInvocationWriter.hpp"
#include "transform/writers/memberAccessWriter.hpp"
#include "transform/writers/statement/plainStatementWriter.hpp"
#include "transform/writers/statement/returnStatementWriter.hpp"
#include "transform/writers/textWriter.hpp"

using mapr::transform::RecordMethodDefinitionMapper;

RecordMethodDefinitionMapper::RecordMethodDefinitionMapper(
	std::shared_ptr<const view::RecordMethodDecl> decl,
	std::shared_ptr<config::PipelineContext> context)
	: FunctionLikeDefinitionMapper(std::move(decl), std::move(context)) {}
void RecordMethodDefinitionMapper::writeOverloadBody(
	std::unique_ptr<BlockWriter>& block,
	const mapr::view::FunctionOverload& overload) {
	auto invocationArgs = std::vector<std::unique_ptr<WriterBase>>();

	auto thisArg = overload.getArguments().front();

	auto thisArgWriter = writeArgument(thisArg);

	std::transform(
		std::begin(overload.getArguments()) + 1,
		std::end(overload.getArguments()),
		std::back_inserter(invocationArgs),
		[this](const std::shared_ptr<const view::VarDecl>& arg)
			-> std::unique_ptr<WriterBase> { return writeArgument(arg); }  //
	);

	auto funInvocation = std::make_unique<FunctionInvocationWriter>(
		std::make_unique<TextWriter>(getDecl()->getQualifiedName().str()),
		std::move(invocationArgs),
		InvocationStyle::NoParens  //
	);

	auto memberInvocation =
		std::make_unique<MemberAccessWriter>(std::move(thisArgWriter),
	                                         std::move(funInvocation),
	                                         MemberAccessMode::PointerLike);

	if (overload.getReturnType()->isVoid()) {
		(*block) << std::make_unique<PlainStatementWriter>(
			std::move(memberInvocation));
	} else {
		(*block) << std::make_unique<ReturnStatementWriter>(writeReturnValue(
			overload.getReturnType(), std::move(memberInvocation)));
	}
}
