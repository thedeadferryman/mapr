//
// Created by kmeinkopf on 27.01.2022.
//

#pragma once

#include <memory>

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class LineEscapedWriter : public WriterBase {
	std::unique_ptr<WriterBase> subWriter;

  public:
	LineEscapedWriter(std::unique_ptr<WriterBase> subWriter);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::getName