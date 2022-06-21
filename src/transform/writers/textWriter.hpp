//
// Created by kmeinkopf on 20.01.2022.
//

#pragma once

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class TextWriter : public WriterBase {
  public:
	static TextWriter const Newline;

  private:
	std::string content;

  public:
	explicit TextWriter(std::string_view content) noexcept;

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform