//
// Created by kmeinkopf on 15.01.2022.
//

#include "writerBase.hpp"

using mapr::transform::WriterBase;

namespace mapr::transform {

auto operator<<(std::ostream& stream, const std::unique_ptr<WriterBase>& writer)
	-> decltype(stream) {
	writer->apply(stream);
	return stream;
}

auto operator<<(std::ostream& stream, const WriterBase& writer)
	-> decltype(stream) {
	writer.apply(stream);
	return stream;
}

}  // namespace mapr::transform