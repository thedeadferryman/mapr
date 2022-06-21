//
// Created by karl on 22.03.22.
//

#pragma once

#include "app/pipeline/pipelineBase.hpp"

namespace mapr::app {

class DefinitionsPipeline : public PipelineBase {
  public:
	DefinitionsPipeline(
		std::shared_ptr<config::PipelineContext> context,
		std::shared_ptr<transform::SourceTransformer> transformer);

	void initialize() override;
	void execute(
		const std::shared_ptr<view::DeclContext>& declContext) override;
};

}  // namespace mapr::app


