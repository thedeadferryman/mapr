//
// Created by kmeinkopf on 23.02.22.
//

#pragma once

#include "transform/mappers/base/mapperBase.hpp"

namespace mapr::transform {

class SourceTransformer;

enum class MapperFactoryMode : std::uint8_t {
	Declaration,
	Definition,
	Database
};

class MapperFactoryBase {
  public:
	MapperFactoryBase(const MapperFactoryBase&) = delete;
	MapperFactoryBase(MapperFactoryBase&&) = delete;
	MapperFactoryBase() = default;

	auto operator=(const MapperFactoryBase&) -> MapperFactoryBase& = delete;
	auto operator=(MapperFactoryBase&&) -> MapperFactoryBase& = delete;

	virtual ~MapperFactoryBase() = default;

	[[nodiscard]] virtual auto acceptsDecl(
		const std::shared_ptr<const view::DeclBase>& decl) const -> bool = 0;

	[[nodiscard]] virtual auto mapperForDecl(
		std::shared_ptr<const view::DeclBase> decl) const
		-> std::unique_ptr<MapperBase> = 0;
};

}  // namespace mapr::transform
