//
// Created by karl on 01.04.22.
//

#pragma once

#include "transform/mappers/common/functionLikeDefinitionMapper.hpp"

namespace mapr {
namespace transform {

class RecordCtorDefinitionMapper : public FunctionLikeDefinitionMapper {
  public:
	RecordCtorDefinitionMapper(
		std::shared_ptr<const view::FunctionDecl> functionDecl,
		std::shared_ptr<config::PipelineContext> context);

  protected:
	void writeOverloadBody(std::unique_ptr<BlockWriter>& block,
	                       const view::FunctionOverload& overload) override;
};

}  // namespace transform
}  // namespace mapr
