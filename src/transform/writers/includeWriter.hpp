//
// Created by kmeinkopf on 26.01.2022.
//

#pragma once

#include "transform/writers/writerBase.hpp"

namespace kodgen::transform {

enum class IncludeType {
	System,
	Custom
};

class IncludeWriter : public WriterBase {
	std::string file;
	IncludeType type;

  public:
	IncludeWriter(std::string_view file,
	              IncludeType type = IncludeType::System);

	void apply(std::ostream& stream) const override;

  private:
	[[nodiscard]] static auto getTypeArgEncloser(IncludeType type)
		-> std::pair<std::string, std::string>;
};

}  // namespace kodgen::getName