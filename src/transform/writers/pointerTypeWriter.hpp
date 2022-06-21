//
// Created by karl on 31.03.22.
//

#pragma once

#include "view/types/pointerType.hpp"

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class PointerTypeWriter : public WriterBase {
	clang::Qualifiers qualifiers;
	std::unique_ptr<WriterBase> pointee;

  public:
	PointerTypeWriter(const clang::Qualifiers &quals, std::unique_ptr<WriterBase> pointee);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform
