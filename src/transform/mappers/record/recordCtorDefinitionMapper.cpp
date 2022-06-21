//
// Created by karl on 01.04.22.
//

#include "recordCtorDefinitionMapper.hpp"

#include "transform/name/typeNameTransformer.hpp"
#include "transform/writers/cxxCastWriter.hpp"
#include "transform/writers/dynCtorInvocationWriter.hpp"
#include "transform/writers/statement/returnStatementWriter.hpp"
#include "transform/writers/textWriter.hpp"

using mapr::transform::RecordCtorDefinitionMapper;

RecordCtorDefinitionMapper::RecordCtorDefinitionMapper(
	std::shared_ptr<const view::FunctionDecl> functionDecl,
	std::shared_ptr<config::PipelineContext> context)
	: FunctionLikeDefinitionMapper(std::move(functionDecl),
                                   std::move(context)) {}

void RecordCtorDefinitionMapper::writeOverloadBody(
	std::unique_ptr<BlockWriter>& block,
	const view::FunctionOverload& overload) {
	auto typeTransformer = TypeNameTransformer(getContext());
	auto invocationArgs = std::vector<std::unique_ptr<WriterBase>>();

	std::transform(std::begin(overload.getArguments()),
	               std::end(overload.getArguments()),
	               std::back_inserter(invocationArgs),
	               [this](const std::shared_ptr<const view::VarDecl>& arg) {
					   return writeArgument(arg);
				   });

	auto className = getDecl()->getQualifiedName().parent();

	auto ctorInvocation = std::make_unique<DynCtorInvocationWriter>(
		std::make_unique<TextWriter>(className.str()),
		std::move(invocationArgs));

	auto returnValue =
		std::make_unique<CXXCastWriter>(  //
			std::move(ctorInvocation),
			std::make_unique<TextWriter>(
				typeTransformer.buildMappedType(overload.getReturnType())),
			CastStyle::ReinterpretCast  //
		);

	(*block) << std::make_unique<ReturnStatementWriter>(std::move(returnValue));
}
