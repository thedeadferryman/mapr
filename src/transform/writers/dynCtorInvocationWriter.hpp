//
// Created by karl on 01.04.22.
//

#pragma once

#include <vector>

#include "transform/writers/function/functionInvocationWriter.hpp"
#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class DynCtorInvocationWriter : public WriterBase {
	std::unique_ptr<WriterBase> className;
	std::vector<std::unique_ptr<WriterBase>> args;

  public:
	DynCtorInvocationWriter(
		std::unique_ptr<WriterBase> className,
		std::vector<std::unique_ptr<WriterBase>> args);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform
