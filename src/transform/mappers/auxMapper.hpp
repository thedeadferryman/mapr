//
// Created by kmeinkopf on 20.01.2022.
//

#pragma once

#include "transform/aux/auxDecl.hpp"
#include "transform/mappers/mapperBase.hpp"

namespace kodgen::transform {

class AuxMapper : public MapperBase {
	std::shared_ptr<AuxDecl> decl;

  public:
	explicit AuxMapper(std::shared_ptr<AuxDecl> decl);

	auto checkDependencies() const -> std::vector<std::shared_ptr<DependencyRequest>> override;
	void writeDeclaration(WriterStream& writer) override;
	void writeDefinition(WriterStream& writer) override;
};

}  // namespace kodgen::getName