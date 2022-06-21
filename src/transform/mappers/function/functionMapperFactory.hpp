//
// Created by kmeinkopf on 23.02.22.
//

#pragma once

#include "transform/mappers/base/mapperFactoryBase.hpp"

#include "config/pipelineContext.hpp"

namespace mapr::transform {

class FunctionMapperFactory : public MapperFactoryBase {
	MapperFactoryMode mode;
	std::shared_ptr<config::PipelineContext> context;

  public:
	FunctionMapperFactory(MapperFactoryMode mode,
	                      std::shared_ptr<config::PipelineContext> context);

	[[nodiscard]] auto acceptsDecl(
		const std::shared_ptr<const view::DeclBase>& decl) const -> bool override;

	[[nodiscard]] auto mapperForDecl(std::shared_ptr<const view::DeclBase> decl) const
		-> std::unique_ptr<MapperBase> override;
};

}  // namespace mapr::transform