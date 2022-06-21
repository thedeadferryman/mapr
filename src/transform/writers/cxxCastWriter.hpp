//
// Created by karl on 22.03.22.
//

#pragma once

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

enum class CastStyle {
	StaticCast,
	ConstCast,
	DynamicCast,
	ReinterpretCast
};

class CXXCastWriter : public WriterBase {
	std::unique_ptr<WriterBase> value;
	std::unique_ptr<WriterBase> type;
	CastStyle style;

  public:
	CXXCastWriter(std::unique_ptr<WriterBase> value,
	              std::unique_ptr<WriterBase> type,
	              CastStyle style = CastStyle::StaticCast);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform
