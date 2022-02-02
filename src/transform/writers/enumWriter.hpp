//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include <memory>
#include <vector>

#include "transform/writers/writerBase.hpp"

namespace kodgen::transform {

class EnumWriter : public WriterBase {
	std::unique_ptr<WriterBase> name;
	std::vector<std::unique_ptr<WriterBase>> members;

  public:
	explicit EnumWriter(std::unique_ptr<WriterBase> name);

	void apply(std::ostream& stream) const override;

	[[nodiscard]] auto operator<<(std::unique_ptr<WriterBase> member) -> EnumWriter&;
};

}  // namespace kodgen::transform