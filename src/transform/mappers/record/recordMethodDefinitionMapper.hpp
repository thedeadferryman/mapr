//
// Created by karl on 31.03.22.
//

#pragma once

#include "view/record/recordMethodDecl.hpp"

#include "transform/writers/writerBase.hpp"

#include "transform/mappers/common/functionLikeDefinitionMapper.hpp"

namespace mapr::transform {

class RecordMethodDefinitionMapper : public FunctionLikeDefinitionMapper {
  public:
	RecordMethodDefinitionMapper(
		std::shared_ptr<const view::RecordMethodDecl> decl,
		std::shared_ptr<config::PipelineContext> context);

  protected:
	void writeOverloadBody(std::unique_ptr<BlockWriter>& block,
	                       const view::FunctionOverload& overload) override;
};

}  // namespace mapr::transform
