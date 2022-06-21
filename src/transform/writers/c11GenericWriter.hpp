//
// Created by kmeinkopf on 26.01.2022.
//

#pragma once

#include "view/types/typeBase.hpp"

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

class C11GenericWriter : public WriterBase {
  public:
	struct Case {
		std::unique_ptr<WriterBase> type;
		std::unique_ptr<WriterBase> value;
	};

  private:
	std::vector<Case> cases;
	std::unique_ptr<WriterBase> defaultCase;
	std::unique_ptr<WriterBase> condition;

  public:
	template<std::derived_from<Case>... Cases>
	C11GenericWriter(std::unique_ptr<WriterBase> defaultCase,
	                 std::unique_ptr<WriterBase> condition,
	                 Cases... cases)
		: cases(cases...)
		, defaultCase(std::move(defaultCase))
		, condition(std::move(condition)) {}

	C11GenericWriter(std::unique_ptr<WriterBase> defaultCase,
	                 std::unique_ptr<WriterBase> condition,
	                 std::vector<Case> cases);

  public:
	void apply(std::ostream& stream) const override;
};

}  // namespace mapr::getName