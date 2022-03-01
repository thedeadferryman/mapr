//
// Created by kmeinkopf on 26.01.2022.
//

#pragma once

#include "view/types/builtinType.hpp"
#include "view/types/typeBase.hpp"

#include "transform/aux/typeDecl.hpp"
#include "transform/mappers/base/mapperBase.hpp"

namespace kodgen::transform {

class TypeMapper : public MapperBase {
	std::shared_ptr<view::TypeBase> type;

  public:
	explicit TypeMapper(const std::shared_ptr<TypeDecl>& decl);

	auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;

	void write(WriterStream& writer) override;

  private:
	static void writeBuiltinType(WriterStream& writer,
	                      const std::shared_ptr<view::BuiltinType>&);
};

}  // namespace kodgen::getName