//
// Created by karl on 21.03.22.
//

#pragma once

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class ReturnStatementWriter : public WriterBase {
	std::unique_ptr<WriterBase> value;

  public:
	explicit ReturnStatementWriter(std::unique_ptr<WriterBase> value = nullptr);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform


