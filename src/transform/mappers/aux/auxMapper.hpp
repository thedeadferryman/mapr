//
// Created by kmeinkopf on 20.01.2022.
//

#pragma once

#include "transform/aux/auxDecl.hpp"

#include "transform/mappers/base/mapperBase.hpp"

namespace mapr::transform {

class AuxMapper : public MapperBase {
	std::shared_ptr<const AuxDecl> decl;
	std::shared_ptr<config::PipelineContext> context;

  public:
	explicit AuxMapper(std::shared_ptr<const AuxDecl> decl,
	                   std::shared_ptr<config::PipelineContext> context);

	[[nodiscard]] auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;

	void write(WriterStream& writer) override;
};

}  // namespace mapr::transform