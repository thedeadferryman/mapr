//
// Created by karl on 14.05.22.
//

#pragma once

#include "view/record/recordDecl.hpp"

#include "transform/mappers/base/mapperBase.hpp"

namespace mapr::transform {

class RecordDefinitionMapper : public MapperBase {
	std::shared_ptr<const view::RecordDecl> decl;

  public:
	explicit RecordDefinitionMapper(std::shared_ptr<const view::RecordDecl> decl);

	void write(WriterStream& writer) override;

	auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;
};

}  // namespace mapr::transform
