//
// Created by karl on 01.04.22.
//

#pragma once

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class DeleteWriter : public WriterBase {
	std::unique_ptr<WriterBase> object;

  public:
	explicit DeleteWriter(std::unique_ptr<WriterBase> object);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform
