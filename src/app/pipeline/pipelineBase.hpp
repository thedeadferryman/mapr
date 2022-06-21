//
// Created by karl on 22.03.22.
//

#pragma once

#include "view/declContext.hpp"

#include "transform/sourceTransformer.hpp"

#include "config/pipelineContext.hpp"

#include "filtering/filterChain.hpp"

namespace mapr::app {

class PipelineBase {
	std::shared_ptr<config::PipelineContext> context;
	std::shared_ptr<transform::SourceTransformer> transformer;
	FilterChain filterChain;
	std::string name;

  public:
	explicit PipelineBase(
		std::shared_ptr<config::PipelineContext> context,
		std::shared_ptr<transform::SourceTransformer> transformer,
		std::string_view name = "<anonymous>");

	PipelineBase(const PipelineBase&) = delete;
	PipelineBase(PipelineBase&&) = delete;

	auto operator=(const PipelineBase&) -> PipelineBase& = delete;
	auto operator=(PipelineBase&&) -> PipelineBase& = delete;

	virtual ~PipelineBase() = default;

	virtual void initialize() = 0;

	[[nodiscard]] auto getTransformer() const
		-> std::shared_ptr<transform::SourceTransformer>;

	[[nodiscard]] auto getContext() const
		-> std::shared_ptr<config::PipelineContext>;

	[[nodiscard]] auto getName() const -> std::string_view;

	virtual void execute(const std::shared_ptr<view::DeclContext>& declContext);
};

}  // namespace mapr::app


