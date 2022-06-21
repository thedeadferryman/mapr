//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include "view/types.hpp"
#include "view/types/recordType.hpp"

#include "config/pipelineContext.hpp"

#include "transform/name/qualifiedNameTransformer.hpp"

namespace mapr::transform {

class TypeNameTransformer {
	enum class QualifiersMode {
		Keep,
		Remove
	};

	std::shared_ptr<config::PipelineContext> context;
	QualifiedNameTransformer nameTransformer;

  public:
	explicit TypeNameTransformer(
		std::shared_ptr<config::PipelineContext> context);

	[[nodiscard]] auto getTypeName(std::shared_ptr<view::TypeBase> type) const
		-> std::string;

	[[nodiscard]] auto buildMappedType(
		std::shared_ptr<view::TypeBase> type) const -> std::string;

	[[nodiscard]] auto getOverloadSlug(
		const std::shared_ptr<view::TypeBase>& type) const -> std::string;

  private:
	[[nodiscard]] auto getUnqualifiedOverloadSlug(
		std::shared_ptr<view::TypeBase> type) const -> std::string;

	[[nodiscard]] auto getBuiltinOverloadSlug(
		const std::shared_ptr<view::BuiltinType>& builtinType) const
		-> std::string;

	[[nodiscard]] auto referenceKindSuffix(
		mapr::view::ReferenceKind kind) const -> std::string_view;

	[[nodiscard]] auto getBuiltinTypeName(
		const std::shared_ptr<view::BuiltinType>& builtinType,
		QualifiersMode qualifiersMode = QualifiersMode::Keep
		) const
		-> std::string;

	[[nodiscard]] auto getPointerTypeName(
		const std::shared_ptr<view::PointerType>& pointerType) const
		-> std::string;

	[[nodiscard]] auto getReferenceTypeName(
		const std::shared_ptr<view::ReferenceType>& referenceType) const
		-> std::string;

	[[nodiscard]] auto getEnumTypeName(
		const std::shared_ptr<view::EnumType>& enumType) const -> std::string;

	[[nodiscard]] auto getRecordTypeName(
		const std::shared_ptr<view::RecordType>& recordType) const
		-> std::string;
};

}  // namespace mapr::transform
