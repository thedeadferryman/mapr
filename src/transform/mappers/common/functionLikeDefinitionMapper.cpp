//
// Created by karl on 31.03.22.
//

#include "functionLikeDefinitionMapper.hpp"

#include <llvm/Support/raw_ostream.h>

#include "transform/dependencies/typeRequest.hpp"
#include "transform/name/functionNameTransformer.hpp"
#include "transform/name/typeNameTransformer.hpp"
#include "transform/typecastHelper.hpp"
#include "transform/writers/cxxCastWriter.hpp"
#include "transform/writers/function/functionHeadWriter.hpp"
#include "transform/writers/sequentialWriter.hpp"
#include "transform/writers/textWriter.hpp"

#include "util/matchType.hpp"

using mapr::transform::FunctionLikeDefinitionMapper;
using mapr::util::MatchType;
using mapr::view::FunctionOverload;

FunctionLikeDefinitionMapper::FunctionLikeDefinitionMapper(
	std::shared_ptr<const view::FunctionDecl> functionDecl,
	std::shared_ptr<config::PipelineContext> context)
	: decl(std::move(functionDecl))
	, context(std::move(context)) {}

auto FunctionLikeDefinitionMapper::checkDependencies() const
	-> std::vector<std::shared_ptr<DependencyRequest>> {
	auto dependencies = std::vector<std::shared_ptr<DependencyRequest>>();

	for (const auto& overload : decl->getOverloads()) {
		dependencies.push_back(
			std::make_shared<TypeRequest>(overload.getReturnType()));

		for (const auto& arg : overload.getArguments()) {
			dependencies.push_back(
				std::make_shared<TypeRequest>(arg->getType()));
		}
	}

	return dependencies;
}

void FunctionLikeDefinitionMapper::write(WriterStream& writer) {
	if (decl->getOverloads().size() > 1) {
		for (const auto& overload : decl->getOverloads()) {
			writer << *writeOverload(FunctionNameTransformer::getOverloadName(
										 context, decl, overload),
			                         overload)
				   << TextWriter("\n\n");
		}
	} else {
		writer << *writeOverload(  //
			FunctionNameTransformer::getOverloadName(context, decl),
			decl->getOverloads().front())
			   << TextWriter("\n\n");
	}
}

auto FunctionLikeDefinitionMapper::getDecl()
	-> const std::shared_ptr<const view::FunctionDecl>& {
	return decl;
}

auto FunctionLikeDefinitionMapper::getContext()
	-> const std::shared_ptr<config::PipelineContext>& {
	return context;
}

auto FunctionLikeDefinitionMapper::writeOverload(
	std::string_view functionName, const FunctionOverload& overload)
	-> std::unique_ptr<WriterBase> {
	auto typeTransformer = TypeNameTransformer(context);

	auto returnType = typeTransformer.buildMappedType(overload.getReturnType());
	auto args = std::vector<FunctionHeadWriter::Argument>();

	for (const auto& arg : overload.getArguments()) {
		const auto& type = typeTransformer.buildMappedType(arg->getType());

		args.push_back(FunctionHeadWriter::Argument {
			.name = arg->getID(),
			.type = type,
		});
	}

	auto head = std::make_unique<FunctionHeadWriter>(
		context,
		std::make_unique<TextWriter>(functionName),
		std::make_unique<TextWriter>(returnType),
		std::move(args),
		FunctionHeadWriter::Mode::SplitDefinition  //
	);

	auto block = std::make_unique<BlockWriter>(std::move(head));

	writeOverloadBody(block, overload);

	return block;
}

auto FunctionLikeDefinitionMapper::writeArgument(
	const std::shared_ptr<const view::VarDecl>& arg) -> std::unique_ptr<WriterBase> {
	return TypecastHelper(context).castFromMapping(
		arg->getType(), std::make_unique<TextWriter>(arg->getID()));
}

auto FunctionLikeDefinitionMapper::writeReturnValue(
	const std::shared_ptr<view::TypeBase>& returnType,
	std::unique_ptr<WriterBase> value) -> std::unique_ptr<WriterBase> {
	return TypecastHelper(context).castToMapping(returnType, std::move(value));
}