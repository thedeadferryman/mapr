//
// Created by karl on 22.03.22.
//

#pragma once

#include "transform/mappers/base/mapperFactoryBase.hpp"

namespace mapr::transform {

class TypeMapperFactory : public MapperFactoryBase {
	std::shared_ptr<config::PipelineContext> context;
	MapperFactoryMode mode;

  public:
	explicit TypeMapperFactory(
		std::shared_ptr<config::PipelineContext> context, MapperFactoryMode mode);

	auto acceptsDecl(const std::shared_ptr<const view::DeclBase>& decl) const
		-> bool override;
	auto mapperForDecl(std::shared_ptr<const view::DeclBase> decl) const
		-> std::unique_ptr<MapperBase> override;
};

}  // namespace mapr::transform


