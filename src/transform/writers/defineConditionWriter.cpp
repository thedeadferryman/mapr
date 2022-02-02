//
// Created by kmeinkopf on 26.01.2022.
//

#include "defineConditionWriter.hpp"

#include "transform/writers/textWriter.hpp"

#include "util/macro.hpp"

using kodgen::transform::DefineConditionWriter;

DefineConditionWriter::DefineConditionWriter(
	std::unique_ptr<const WriterBase> condition,
	std::unique_ptr<const WriterBase> thenBranch,
	std::unique_ptr<const WriterBase> elseBranch,
	ConditionType type)
	: type(type)
	, condition(std::move(condition))
	, thenBranch(std::move(thenBranch))
	, elseBranch(std::move(elseBranch)) {}

auto DefineConditionWriter::ifCXX(std::unique_ptr<const WriterBase> thenBranch,
                                  std::unique_ptr<const WriterBase> elseBranch,
                                  bool isInverted) -> DefineConditionWriter {
	return DefineConditionWriter(
		std::make_unique<TextWriter>("__cplusplus"),
		std::move(thenBranch),
		std::move(elseBranch),
		(isInverted ? ConditionType::IfUndefined : ConditionType::IfDefined));
}

void DefineConditionWriter::apply(std::ostream& stream) const {
	stream << getKeywordForType(type) << ' ';

	condition->apply(stream);

	stream << "\n";

	if (thenBranch) {
		thenBranch->apply(stream);
	}

	if (elseBranch) {
		stream << "\n#else //";

		condition->apply(stream);
		stream << "\n";

		elseBranch->apply(stream);
	}

	stream << "\n#endif //";

	condition->apply(stream);
	stream << "\n";
}

auto DefineConditionWriter::getKeywordForType(ConditionType type)
	-> std::string {
	switch (type) {
		case ConditionType::Normal:
			return "#if";
		case ConditionType::IfDefined:
			return "#ifdef";
		case ConditionType::IfUndefined:
			return "#ifndef";
		default:
			UNREACHABLE();
	}
}
