//
// Created by kmeinkopf on 15.01.2022.
//

#pragma once

#include <memory>
#include <ostream>

#include "transform/writerStream.hpp"

namespace mapr::transform {

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

auto operator<<(std::ostream& stream, const WriterBase& writer)
	-> decltype(stream);
auto operator<<(std::ostream& stream, const std::unique_ptr<WriterBase>& writer)
	-> decltype(stream);

}  // namespace mapr::transform
