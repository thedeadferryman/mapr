//
// Created by kmeinkopf on 27.01.2022.
//

#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class DefineWriter : public WriterBase {
	std::unique_ptr<WriterBase> idWriter;
	std::optional<std::vector<std::unique_ptr<WriterBase>>> argWriters;
	std::unique_ptr<WriterBase> bodyWriter;

  public:
	DefineWriter(
		std::unique_ptr<WriterBase> idWriter,
		std::optional<std::vector<std::unique_ptr<WriterBase>>> argWriters,
		std::unique_ptr<WriterBase> bodyWriter);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform