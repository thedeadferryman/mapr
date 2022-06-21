//
// Created by karl on 21.03.22.
//

#include <utility>

#include "functionDefinitionMapper.hpp"

#include "transform/name/functionNameTransformer.hpp"
#include "transform/writers/blockWriter.hpp"
#include "transform/writers/cxxCastWriter.hpp"
#include "transform/writers/function/functionInvocationWriter.hpp"
#include "transform/writers/statement/plainStatementWriter.hpp"
#include "transform/writers/statement/returnStatementWriter.hpp"
#include "transform/writers/textWriter.hpp"

using mapr::transform::FunctionDefinitionMapper;
using mapr::view::FunctionOverload;

FunctionDefinitionMapper::FunctionDefinitionMapper(
	std::shared_ptr<const view::FunctionDecl> functionDecl,
	std::shared_ptr<config::PipelineContext> context)
	: FunctionLikeDefinitionMapper(std::move(functionDecl),
                                   std::move(context)) {}

void FunctionDefinitionMapper::writeOverloadBody(
	std::unique_ptr<BlockWriter>& block, const FunctionOverload& overload) {
	auto invocationArgs = std::vector<std::unique_ptr<WriterBase>>();

	std::transform(std::begin(overload.getArguments()),
	               std::end(overload.getArguments()),
	               std::back_inserter(invocationArgs),
	               [this](const std::shared_ptr<const view::VarDecl>& varDecl) {
					   return writeArgument(varDecl);
				   });

	auto originalInvocation = writeReturnValue(
		overload.getReturnType(),
		std::make_unique<FunctionInvocationWriter>(
			std::make_unique<TextWriter>(getDecl()->getQualifiedName().str()),
			std::move(invocationArgs)));

	if (overload.getReturnType()->isVoid()) {
		(*block) << std::make_unique<PlainStatementWriter>(
			std::move(originalInvocation));
	} else {
		(*block) << std::make_unique<ReturnStatementWriter>(
			std::move(originalInvocation));
	}
}
