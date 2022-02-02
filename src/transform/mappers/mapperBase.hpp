//
// Created by kmeinkopf on 26.12.2021.
//

#pragma once

#include <string>
#include <variant>
#include <vector>

#include "view/declBase.hpp"

#include "transform/dependencies/dependencyResolver.hpp"

namespace kodgen::transform {

class WriterStream;

class MapperBase {
  public:
	MapperBase() = default;

	MapperBase(const MapperBase&) = default;
	MapperBase(MapperBase&&) = delete;

	auto operator=(const MapperBase&) -> MapperBase& = delete;
	auto operator=(MapperBase&&) -> MapperBase& = delete;

	virtual ~MapperBase() = default;

	[[nodiscard]] virtual auto checkDependencies() const -> std::vector<std::shared_ptr<DependencyRequest>> = 0;

	virtual void writeDeclaration(WriterStream& writer) = 0;

	virtual void writeDefinition(WriterStream& writer) = 0;
};

}  // namespace kodgen::getName