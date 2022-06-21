//
// Created by karl on 01.04.22.
//

#pragma once

#include "transform/writers/statement/plainStatementWriter.hpp"
#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class AssignmentWriter : public WriterBase {
	std::unique_ptr<WriterBase> receiver;
	std::unique_ptr<WriterBase> value;

  public:
	[[nodiscard]] static auto statement(std::unique_ptr<WriterBase> receiver,
	                                    std::unique_ptr<WriterBase> value)
		-> std::unique_ptr<PlainStatementWriter>;

	AssignmentWriter(std::unique_ptr<WriterBase> receiver,
	                 std::unique_ptr<WriterBase> value);

	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::transform
