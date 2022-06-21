//
// Created by kmeinkopf on 26.01.2022.
//

#pragma once

#include <memory>
#include <optional>

#include "transform/writers/writerBase.hpp"

namespace mapr::transform {

enum class ConditionType {
	Normal,
	IfDefined,
	IfUndefined,
};

class DefineConditionWriter : public WriterBase {
	ConditionType type;
	std::unique_ptr<const WriterBase> condition;
	std::unique_ptr<const WriterBase> thenBranch;
	std::unique_ptr<const WriterBase> elseBranch;

  public:
	static auto ifCXX(std::unique_ptr<const WriterBase> thenBranch,
	                  std::unique_ptr<const WriterBase> elseBranch,
	                  ConditionType type) -> DefineConditionWriter;

  public:
	DefineConditionWriter(
		std::unique_ptr<const WriterBase> condition,
		std::unique_ptr<const WriterBase> thenBranch,
		std::unique_ptr<const WriterBase> elseBranch = nullptr,
		ConditionType type = ConditionType::Normal);

	void apply(std::ostream& stream) const override;

  private:
	[[nodiscard]] static auto getKeywordForType(ConditionType type)
		-> std::string;
};

}  // namespace mapr::getName