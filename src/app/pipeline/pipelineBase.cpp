//
// Created by karl on 22.03.22.
//

#include <utility>

#include "pipelineBase.hpp"

#include <llvm/Support/raw_ostream.h>

using mapr::app::PipelineBase;

PipelineBase::PipelineBase(
	std::shared_ptr<config::PipelineContext> properties,
	std::shared_ptr<transform::SourceTransformer> transformer,
	std::string_view name)
	: context(std::move(properties))
	, transformer(std::move(transformer))
	, filterChain(context->getConfig().filters)
	, name(name) {}

auto PipelineBase::getTransformer() const
	-> std::shared_ptr<transform::SourceTransformer> {
	return transformer;
}

auto PipelineBase::getContext() const
	-> std::shared_ptr<config::PipelineContext> {
	return context;
}

void PipelineBase::execute(
	const std::shared_ptr<view::DeclContext>& declContext) {
	for (const auto& [id, decl] : declContext->getDeclarations()) {
		auto filterRes = filterChain.checkDecl(decl);

		if (config::isSparseResult(filterRes)) {
			auto mode = config::sparseResultToFilteringMode(filterRes);

			transformer->writeDecl(  //
				filterChain.filterSparseDecl(decl, mode),
				name  //
			);
		} else if (filterRes == config::FilterResult::Include) {
			transformer->writeDecl(decl, name);
		}
	}
}
auto mapr::app::PipelineBase::getName() const -> std::string_view {
	return name;
}
