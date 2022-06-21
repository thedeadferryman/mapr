//
// Created by karl on 31.03.22.
//

#pragma once

#include "view/record/recordDecl.hpp"

#include "transform/mappers/base/mapperBase.hpp"

namespace mapr::transform {

class RecordDeclarationMapper : public MapperBase {
	std::shared_ptr<config::PipelineContext> context;
	std::shared_ptr<const view::RecordDecl> decl;

  public:
	RecordDeclarationMapper(std::shared_ptr<const view::RecordDecl> decl,
	                        std::shared_ptr<config::PipelineContext> context);

	[[nodiscard]] auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;

	void write(WriterStream& writer) override;
};

}  // namespace mapr::transform
