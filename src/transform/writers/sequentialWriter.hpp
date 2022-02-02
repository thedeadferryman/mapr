//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "transform/writers/writerBase.hpp"

namespace kodgen::transform {

class SequentialWriter : public WriterBase {
	std::vector<std::unique_ptr<WriterBase>> writers;

  public:
	[[nodiscard]] static auto joinToPtr(
		std::vector<std::unique_ptr<WriterBase>> writers,
		std::function<std::unique_ptr<WriterBase>()> glue) -> std::unique_ptr<SequentialWriter>;

	explicit SequentialWriter(std::vector<std::unique_ptr<WriterBase>> writers);

	void apply(std::ostream& stream) const override;
};

}  // namespace kodgen::transform