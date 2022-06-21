//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include "view/function/functionDecl.hpp"

#include "config/pipelineContext.hpp"

namespace mapr::transform {

class FunctionNameTransformer {
  public:
	enum class SlugType {
		NoSlug,
		CountSlug,
		FullSlug
	};

	FunctionNameTransformer() = delete;

	[[nodiscard]] static auto getOverloadName(
		const std::shared_ptr<config::PipelineContext>& context,
		const std::shared_ptr<const view::FunctionDecl>& functionDecl) -> std::string;

	[[nodiscard]] static auto getOverloadName(
		const std::shared_ptr<config::PipelineContext>& context,
		const std::shared_ptr<const view::FunctionDecl>& functionDecl,
		std::size_t argCount) -> std::string;

	[[nodiscard]] static auto getOverloadName(
		const std::shared_ptr<config::PipelineContext>& context,
		const std::shared_ptr<const view::FunctionDecl>& functionDecl,
		const view::FunctionOverload& overload) -> std::string;

	[[nodiscard]] static auto getOverloadName(
		const std::shared_ptr<config::PipelineContext>& context,
		const std::shared_ptr<const view::FunctionDecl>& functionDecl,
		const view::FunctionOverload& overload,
		SlugType slugType) -> std::string;

  private:
	[[nodiscard]] static auto getOverloadTypedSuffix(
		const std::shared_ptr<config::PipelineContext>& context,
		const view::FunctionOverload& overload) -> std::string;

	[[nodiscard]] static auto getOverloadCountSuffix(std::size_t argCount)
		-> std::string;

	[[nodiscard]] static auto getCtorName(
		const std::shared_ptr<config::PipelineContext>& context) -> std::string;
	[[nodiscard]] static auto getDtorName(
		const std::shared_ptr<config::PipelineContext>& context) -> std::string;

	[[nodiscard]] static auto getSyntheticGetterName(
		const std::shared_ptr<config::PipelineContext>& context,
		std::string_view fieldName) -> std::string;
	[[nodiscard]] static auto getSyntheticSetterName(
		const std::shared_ptr<config::PipelineContext>& context,
		std::string_view fieldName) -> std::string;

	[[nodiscard]] static auto getOperatorName(
		const std::shared_ptr<config::PipelineContext>& context,
		const std::shared_ptr<const view::FunctionDecl>& functionDecl) -> std::string;

	[[nodiscard]] static auto getOperatorNameForKind(
		const std::shared_ptr<config::PipelineContext>& context,
		clang::OverloadedOperatorKind operatorKind) -> std::string;

	[[nodiscard]] static auto getOperatorAlias(
		clang::OverloadedOperatorKind operatorKind) -> std::string;
};

}  // namespace mapr::transform
