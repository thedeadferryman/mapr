//
// Created by karl on 06.04.22.
//

#pragma once

#include "view/types/recordType.hpp"
#include "view/types/referenceType.hpp"
#include "view/types/typeBase.hpp"

#include "transform/writers/cxxCastWriter.hpp"

#include "config/pipelineContext.hpp"

namespace mapr::transform {

class TypecastHelper {
	std::shared_ptr<config::PipelineContext> context;

  public:
	explicit TypecastHelper(std::shared_ptr<config::PipelineContext> context);

	[[nodiscard]] auto castToMapping(
		const std::shared_ptr<view::TypeBase>& type,
		std::unique_ptr<WriterBase> value) const -> std::unique_ptr<WriterBase>;

	[[nodiscard]] auto castFromMapping(
		const std::shared_ptr<view::TypeBase>& type,
		std::unique_ptr<WriterBase> value) const -> std::unique_ptr<WriterBase>;

  private:
	[[nodiscard]] auto castReferenceFromMapping(
		const std::shared_ptr<view::ReferenceType>& refType,
		std::unique_ptr<WriterBase> value) const -> std::unique_ptr<WriterBase>;

	[[nodiscard]] auto castReferenceToMapping(
		const std::shared_ptr<view::ReferenceType>& refType,
		std::unique_ptr<WriterBase> value) const -> std::unique_ptr<WriterBase>;

	[[nodiscard]] auto castPointerToMapping(
		const std::shared_ptr<view::PointerType>& refType,
		std::unique_ptr<WriterBase> value) const -> std::unique_ptr<WriterBase>;
};

}  // namespace mapr::transform
