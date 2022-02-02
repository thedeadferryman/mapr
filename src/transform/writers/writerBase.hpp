//
// Created by kmeinkopf on 15.01.2022.
//

#pragma once

#include <ostream>

#include "transform/writerStream.hpp"

namespace kodgen::transform {

class WriterBase {
  public:
	WriterBase() = default;

	WriterBase(const WriterBase&) = delete;
	WriterBase(WriterBase&&) = delete;

	auto operator=(const WriterBase&) -> WriterBase& = delete;
	auto operator=(WriterBase&&) -> WriterBase& = delete;

	virtual ~WriterBase() = default;

	virtual void apply(std::ostream& stream) const = 0;
};

}  // namespace kodgen::getName
