//
// Created by karl on 22.03.22.
//

#pragma once

#include "transform/sourceTransformer.hpp"

#include "app/pipeline/pipelineBase.hpp"

#include "config/pipelineContext.hpp"

namespace mapr::app {

class DeclarationsPipeline : public PipelineBase {
  public:
	DeclarationsPipeline(
		std::shared_ptr<config::PipelineContext> context,
		std::shared_ptr<transform::SourceTransformer> transformer);

	void initialize() override;
};

}  // namespace mapr::app


