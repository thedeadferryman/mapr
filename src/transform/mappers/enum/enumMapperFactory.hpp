//
// Created by kmeinkopf on 23.02.22.
//

#pragma once

#include "transform/mappers/base/mapperFactoryBase.hpp"

namespace kodgen::transform {

class EnumMapperFactory : public MapperFactoryBase {
  public:
	auto acceptsDecl(const std::shared_ptr<view::DeclBase>& decl) const
		-> bool override;

	auto mapperForDecl(std::shared_ptr<view::DeclBase> decl) const
		-> std::unique_ptr<MapperBase> override;
};

}  // namespace kodgen::transform
