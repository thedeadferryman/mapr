//
// Created by karl on 21.03.22.
//

#pragma once

#include "view/function/functionDecl.hpp"

#include "transform/writers/writerBase.hpp"

#include "transform/mappers/base/mapperBase.hpp"
#include "transform/mappers/common/functionLikeDefinitionMapper.hpp"

#include "config/pipelineContext.hpp"

namespace mapr::transform {

class FunctionDefinitionMapper : public FunctionLikeDefinitionMapper {
  public:
	explicit FunctionDefinitionMapper(
		std::shared_ptr<const view::FunctionDecl> functionDecl,
		std::shared_ptr<config::PipelineContext> context);

  protected:
	void writeOverloadBody(std::unique_ptr<BlockWriter>& block,
	                       const view::FunctionOverload& overload) override;
};

}  // namespace mapr::transform
