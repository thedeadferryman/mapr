//
// Created by karl on 21.03.22.
//

#pragma once

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class PlainStatementWriter : public WriterBase {
	std::unique_ptr<WriterBase> body;

  public:
	explicit PlainStatementWriter(std::unique_ptr<WriterBase> body);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform


