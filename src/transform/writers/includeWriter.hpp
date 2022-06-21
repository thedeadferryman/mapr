//
// Created by kmeinkopf on 26.01.2022.
//

#pragma once

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

enum class IncludeMode {
	System,
	Custom
};

class IncludeWriter : public WriterBase {
	std::string file;
	IncludeMode type;

  public:
	IncludeWriter(std::string_view file,
	              IncludeMode type = IncludeMode::System);

	void apply(std::ostream& stream) const override;

  private:
	[[nodiscard]] static auto getTypeArgEncloser(IncludeMode type)
		-> std::pair<std::string, std::string>;
};

}  // namespace mapr::getName