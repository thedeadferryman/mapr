//
// Created by kmeinkopf on 20.01.2022.
//

#pragma once

#include "transform/writers/writerBase.hpp"

namespace kodgen::transform {

class TextWriter : public WriterBase {
	std::string content;

  public:
	explicit TextWriter(std::string_view content);

	void apply(std::ostream& stream) const override;
};

}  // namespace kodgen::getName