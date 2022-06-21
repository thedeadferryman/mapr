//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include "view/enum/enumDecl.hpp"

#include "transform/mappers/base/mapperBase.hpp"

namespace mapr::transform {

class EnumMapper : public MapperBase {
	std::shared_ptr<const view::EnumDecl> enumDecl;
	std::shared_ptr<config::PipelineContext> context;

  public:
	EnumMapper(std::shared_ptr<const view::EnumDecl> enumDecl,
	           std::shared_ptr<config::PipelineContext> context);

	auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;

	void write(WriterStream& writer) override;

  private:
	[[nodiscard]] auto
	getMemberName(  // TODO: think of extracting this logic to a separate module
		const std::shared_ptr<const view::EnumMemberDecl>& member) const
		-> std::string;
};

}  // namespace mapr::transform