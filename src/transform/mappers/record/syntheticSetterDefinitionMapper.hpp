//
// Created by karl on 11.04.22.
//

#pragma once

#include "view/record/syntheticGetterDecl.hpp"

#include "transform/mappers/common/functionLikeDefinitionMapper.hpp"

namespace mapr::transform {

class SyntheticSetterDefinitionMapper : public FunctionLikeDefinitionMapper {
  public:
	explicit SyntheticSetterDefinitionMapper(
		std::shared_ptr<const view::FunctionDecl> decl,
		std::shared_ptr<config::PipelineContext> context);

  protected:
	void writeOverloadBody(std::unique_ptr<BlockWriter>& block,
	                       const view::FunctionOverload& overload) override;
};

}  // namespace mapr::transform
