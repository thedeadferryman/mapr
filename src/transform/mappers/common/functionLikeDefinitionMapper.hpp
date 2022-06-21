//
// Created by karl on 31.03.22.
//

#pragma once

#include "view/function/functionDecl.hpp"

#include "transform/writers/blockWriter.hpp"
#include "transform/writers/writerBase.hpp"

#include "transform/mappers/base/mapperBase.hpp"

namespace mapr::transform {

class FunctionLikeDefinitionMapper : public MapperBase {
	std::shared_ptr<const view::FunctionDecl> decl;
	std::shared_ptr<config::PipelineContext> context;

  public:
	FunctionLikeDefinitionMapper(
		std::shared_ptr<const view::FunctionDecl> functionDecl,
		std::shared_ptr<config::PipelineContext> context);

	[[nodiscard]] auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;

	void write(WriterStream& writer) override;

  protected:
	[[nodiscard]] auto getDecl() -> const std::shared_ptr<const view::FunctionDecl>&;

	[[nodiscard]] auto getContext()
		-> const std::shared_ptr<config::PipelineContext>&;

	virtual void writeOverloadBody(std::unique_ptr<BlockWriter>& block,
	                               const view::FunctionOverload& overload) = 0;

	[[nodiscard]] virtual auto writeOverload(
		std::string_view functionName, const view::FunctionOverload& overload)
		-> std::unique_ptr<WriterBase>;

	[[nodiscard]] virtual auto writeArgument(
		const std::shared_ptr<const view::VarDecl>& arg)
		-> std::unique_ptr<WriterBase>;

	[[nodiscard]] virtual auto writeReturnValue(
		const std::shared_ptr<view::TypeBase>& returnType,
		std::unique_ptr<WriterBase> value) -> std::unique_ptr<WriterBase>;
};

}  // namespace mapr::transform
