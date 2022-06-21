//
// Created by karl on 31.03.22.
//

#pragma once

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

enum class MemberAccessMode {
	Regular,
	PointerLike
};

class MemberAccessWriter : public WriterBase {
	std::unique_ptr<WriterBase> owner;
	std::unique_ptr<WriterBase> target;
	MemberAccessMode accessMode;

  public:
	MemberAccessWriter(std::unique_ptr<WriterBase> owner,
	                   std::unique_ptr<WriterBase> target,
	                   MemberAccessMode accessMode = MemberAccessMode::Regular);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform
