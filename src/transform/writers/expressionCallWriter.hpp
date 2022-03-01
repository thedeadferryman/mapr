//
// Created by kmeinkopf on 01.03.22.
//

#pragma once

#include <memory>
#include <vector>

#include "transform/writers/writerBase.hpp"

namespace kodgen::transform {

class ExpressionCallWriter : public WriterBase {
	std::unique_ptr<WriterBase> expr;
	std::vector<std::unique_ptr<WriterBase>> args;

  public:
	ExpressionCallWriter(std::unique_ptr<WriterBase> expr,
	                     std::vector<std::unique_ptr<WriterBase>> args);

		void apply(std::ostream& stream) const override;
};

}  // namespace kodgen::transform
