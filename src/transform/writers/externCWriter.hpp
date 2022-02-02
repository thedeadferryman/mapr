//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include "transform/writers/blockWriter.hpp"
#include "transform/writers/writerBase.hpp"

namespace kodgen::transform {

class ExternCWriter : public WriterBase {
  private:
	enum class ExternKind {
		OpenBlock,
		CloseBlock,
		Inline
	};

  public:
	static const ExternCWriter Inline;
	[[deprecated]] static const ExternCWriter OpenBlock;
	[[deprecated]] static const ExternCWriter CloseBlock;

	static auto makeBlock() -> BlockWriter;

  private:
	ExternKind kind;

	explicit ExternCWriter(ExternKind kind = ExternKind::Inline) noexcept;

  public:
	void apply(std::ostream& stream) const override;
};

}  // namespace kodgen::getName
