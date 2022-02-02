//
// Created by kmeinkopf on 27.01.2022.
//

#pragma once

#include <memory>
#include <vector>

#include "transform/writers/writerBase.hpp"

namespace kodgen::transform {

class IEEWriter : public WriterBase {
	std::unique_ptr<WriterBase> expr;
	std::vector<std::unique_ptr<WriterBase>> args;

  public:
	explicit IEEWriter(std::unique_ptr<WriterBase> expr,
	                   std::vector<std::unique_ptr<WriterBase>> args);

	void apply(std::ostream& stream) const override;
};

}  // namespace kodgen::getName