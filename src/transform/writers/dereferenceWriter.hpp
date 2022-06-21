//
// Created by karl on 01.04.22.
//

#pragma once

#include "transform/writers/writerBase.hpp"

namespace mapr {
namespace transform {

class DereferenceWriter : public WriterBase {
	std::unique_ptr<WriterBase> value;

  public:
	explicit DereferenceWriter(std::unique_ptr<WriterBase> value);

	void apply(std::ostream& stream) const override;
};

}  // namespace transform
}  // namespace mapr
