//
// Created by karl on 15.05.22.
//

#pragma once

#include "transform/writers/writerBase.hpp"
namespace mapr::transform {

class PointerizeWriter : public WriterBase {
	std::unique_ptr<WriterBase> expr;

  public:
	explicit PointerizeWriter(std::unique_ptr<WriterBase> expr);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform
