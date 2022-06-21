//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include "transform/writers/blockWriter.hpp"
#include "transform/writers/writerBase.hpp"

#include "config/pipelineContext.hpp"

#include "transform/writers/macroBlockWriter.hpp"

namespace mapr::transform {

class ExternCWriter : public WriterBase {
	std::shared_ptr<config::PipelineContext> context;

  public:
	static auto makeBlock(const std::shared_ptr<config::PipelineContext>& context)
		-> mapr::transform::MacroBlockWriter;

	explicit ExternCWriter(std::shared_ptr<config::PipelineContext> context);

	void apply(std::ostream& stream) const override;

  private:
	[[nodiscard]] static auto getExportSpecifier(const std::shared_ptr<config::PipelineContext>& context) -> std::string;
};

}  // namespace mapr::transform
