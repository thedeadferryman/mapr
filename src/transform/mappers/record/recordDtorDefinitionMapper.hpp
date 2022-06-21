//
// Created by karl on 01.04.22.
//

#pragma once

#include "transform/mappers/common/functionLikeDefinitionMapper.hpp"

namespace mapr::transform {

class RecordDtorDefinitionMapper : public FunctionLikeDefinitionMapper {
  public:
	RecordDtorDefinitionMapper(
		std::shared_ptr<const view::FunctionDecl> functionDecl,
		std::shared_ptr<config::PipelineContext> context);

  protected:
	void writeOverloadBody(std::unique_ptr<BlockWriter>& block,
	                       const view::FunctionOverload& overload) override;
};

}  // namespace mapr::transform