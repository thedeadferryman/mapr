//
// Created by karl on 21.03.22.
//

#pragma once

#include "view/name/qualifiedName.hpp"

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

enum class InvocationStyle {
	NoParens,
	ParensAround
};

class FunctionInvocationWriter : public WriterBase {
	std::unique_ptr<WriterBase> name;
	std::vector<std::unique_ptr<WriterBase>> arguments;
	InvocationStyle style;

  public:
	FunctionInvocationWriter(
		std::unique_ptr<WriterBase> name,
		std::vector<std::unique_ptr<WriterBase>> arguments,
		InvocationStyle style = InvocationStyle::ParensAround);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform
