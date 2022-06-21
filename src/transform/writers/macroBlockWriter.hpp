//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include <memory>
#include <vector>

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class MacroBlockWriter : public WriterBase {
	std::unique_ptr<WriterBase> opener;
	std::unique_ptr<WriterBase> closer;
	std::vector<std::unique_ptr<WriterBase>> body;

  public:
	MacroBlockWriter(std::unique_ptr<WriterBase> opener,
	                 std::unique_ptr<WriterBase> closer);

	MacroBlockWriter(std::unique_ptr<WriterBase> opener,
	                 std::unique_ptr<WriterBase> closer,
	                 std::vector<std::unique_ptr<WriterBase>> body);

	void apply(std::ostream& stream) const override;

	auto operator<<(std::unique_ptr<WriterBase> writer) -> MacroBlockWriter&;
};

}  // namespace mapr::transform