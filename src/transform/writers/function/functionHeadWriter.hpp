//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include <memory>
#include <unordered_map>

#include "view/function/functionDecl.hpp"

#include "transform/writers/writerBase.hpp"

#include "config/pipelineContext.hpp"

namespace mapr::transform {

class FunctionHeadWriter : public WriterBase {
  public:
	enum class Mode {
		Declaration,
		SplitDefinition,
		FullDefinition
	};

	struct Argument {
		std::string name;
		std::string type;
	} __attribute__((aligned(64)));

  private:
	Mode mode;
	std::unique_ptr<WriterBase> name;
	std::unique_ptr<WriterBase> returnType;
	std::vector<Argument> arguments;
	std::shared_ptr<config::PipelineContext> context;

  public:
	FunctionHeadWriter(std::shared_ptr<config::PipelineContext> context,
	                   std::unique_ptr<WriterBase> name,
	                   std::unique_ptr<WriterBase> returnType,
	                   std::vector<Argument> arguments,
	                   Mode mode = Mode::Declaration);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform
