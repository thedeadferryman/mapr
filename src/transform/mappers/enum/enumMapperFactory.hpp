//
// Created by kmeinkopf on 23.02.22.
//

#pragma once

#include "transform/mappers/base/mapperFactoryBase.hpp"

namespace mapr::transform {

class EnumMapperFactory : public MapperFactoryBase {
	std::shared_ptr<config::PipelineContext> context;

  public:
	explicit EnumMapperFactory(
		std::shared_ptr<config::PipelineContext> context);

	auto acceptsDecl(const std::shared_ptr<const view::DeclBase>& decl) const
		-> bool override;

	auto mapperForDecl(std::shared_ptr<const view::DeclBase> decl) const
		-> std::unique_ptr<MapperBase> override;
};

}  // namespace mapr::transform
