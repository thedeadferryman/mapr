//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include <memory>
#include <vector>

#include "transform/writers/writerBase.hpp"

namespace kodgen::transform {

class BlockWriter : public WriterBase {
  private:
	std::unique_ptr<WriterBase> prelude;
	std::vector<std::unique_ptr<WriterBase>> body;
	bool semicolon;

  public:
	BlockWriter(std::unique_ptr<WriterBase> prelude,
	            bool semicolon = false);
	BlockWriter(std::unique_ptr<WriterBase> prelude,
	            std::vector<std::unique_ptr<WriterBase>> body,
	            bool semicolon = false);


	void apply(std::ostream& stream) const override;

	auto operator<<(std::unique_ptr<WriterBase>&& writer) -> BlockWriter&;
};

}  // namespace kodgen::transform