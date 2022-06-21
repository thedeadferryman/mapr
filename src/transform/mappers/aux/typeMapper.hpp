//
// Created by kmeinkopf on 26.01.2022.
//

#pragma once

#include "view/types/builtinType.hpp"
#include "view/types/typeBase.hpp"

#include "transform/aux/typeDecl.hpp"

#include "transform/mappers/base/mapperBase.hpp"
#include "transform/mappers/base/mapperFactoryBase.hpp"

namespace mapr::transform {

class TypeMapper : public MapperBase {
	std::shared_ptr<view::TypeBase> type;
	std::shared_ptr<config::PipelineContext> context;
	MapperFactoryMode mode;

  public:
	TypeMapper(const std::shared_ptr<const TypeDecl> &decl,
	           std::shared_ptr<config::PipelineContext> context,
	           MapperFactoryMode mode
	           );

	auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;

	void write(WriterStream& writer) override;

  private:
	static void writeBuiltinType(WriterStream& writer,
	                             const std::shared_ptr<view::BuiltinType>&);

	[[nodiscard]] static auto inferRequiredType(
		const std::shared_ptr<view::TypeBase>& type)
		-> std::shared_ptr<DependencyRequest>;
};

}  // namespace mapr::transform