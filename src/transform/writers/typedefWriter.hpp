//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include <memory>

#include "transform/writers/writerBase.hpp"

namespace kodgen::transform {

class TypedefWriter : public WriterBase {
	std::unique_ptr<WriterBase> origin;
	std::unique_ptr<WriterBase> alias;

  public:
	TypedefWriter(std::unique_ptr<WriterBase> origin,
	              std::unique_ptr<WriterBase> alias);

	void apply(std::ostream& stream) const override;
};

}  // namespace kodgen::getName