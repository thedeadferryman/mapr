//
// Created by kmeinkopf on 15.01.2022.
//

#pragma once

#include <ostream>

namespace mapr::transform {

class WriterBase;

class WriterStream {
	std::ostream& oStream;

  public:
	explicit WriterStream(std::ostream& targetStream);

	void applyWriter(const WriterBase& writer);

	auto operator<<(const WriterBase& writer) -> WriterStream&;
};

}  // namespace mapr::getName
