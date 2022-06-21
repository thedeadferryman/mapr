//
// Created by karl on 31.03.22.
//

#pragma once

#include "transform/mappers/base/mapperFactoryBase.hpp"

namespace mapr::transform {

class RecordMapperFactory : public MapperFactoryBase {
	MapperFactoryMode mode;
	std::shared_ptr<config::PipelineContext> context;

  public:
	RecordMapperFactory(MapperFactoryMode mode,
	                    std::shared_ptr<config::PipelineContext> context);

	[[nodiscard]] auto acceptsDecl(
		const std::shared_ptr<const view::DeclBase>& decl) const -> bool override;

	[[nodiscard]] auto mapperForDecl(std::shared_ptr<const view::DeclBase> decl) const
		-> std::unique_ptr<MapperBase> override;
};

}  // namespace mapr::transform
