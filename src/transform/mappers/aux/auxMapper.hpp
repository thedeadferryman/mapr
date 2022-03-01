//
// Created by kmeinkopf on 20.01.2022.
//

#pragma once

#include "transform/aux/auxDecl.hpp"
#include "transform/mappers/base/mapperBase.hpp"

namespace kodgen::transform {

class AuxMapper : public MapperBase {
	std::shared_ptr<AuxDecl> decl;

  public:
	explicit AuxMapper(std::shared_ptr<AuxDecl> decl);

	[[nodiscard]] auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;

	void write(WriterStream& writer) override;
};

}  // namespace kodgen::transform