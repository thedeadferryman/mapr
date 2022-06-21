//
// Created by kmeinkopf on 26.12.2021.
//

#pragma once

#include <unordered_map>

#include <boost/graph/adjacency_list.hpp>

#include "view/function/functionDecl.hpp"

#include "transform/name/functionNameTransformer.hpp"
#include "transform/writers/writerBase.hpp"

#include "transform/mappers/base/mapperBase.hpp"

#include "util/tree/nodeBase.hpp"

namespace mapr::transform {

class FunctionDeclarationMapper : public MapperBase {
	std::shared_ptr<const view::FunctionDecl> functionDecl;
	std::shared_ptr<tree::NodeBase> overloadTree;
	std::unordered_map<std::size_t, std::size_t> argCounts;
	std::shared_ptr<config::PipelineContext> context;

  public:
	explicit FunctionDeclarationMapper(
		std::shared_ptr<const view::FunctionDecl> functionDecl,
		std::shared_ptr<config::PipelineContext> context);

	[[nodiscard]] auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;

	void write(WriterStream& writer) override;

  private:
	// region writeTypedOverloadDeclarations

	[[nodiscard]] auto writeTypedOverloadDeclarationBody(
		const std::shared_ptr<tree::NodeBase>& node,
		const view::FunctionOverload& path) const
		-> std::unique_ptr<WriterBase>;

	void writeTypedOverloadDeclarations(WriterStream& stream) const;

	// endregion

	void buildOverloadTree(const std::shared_ptr<tree::NodeBase>& node,
	                       const view::FunctionOverload& overload) const;

	void computeArgMap();
	void writeOverloadTree(WriterStream& writer) const;

	auto getTypedOverloadHandlerName(std::size_t argCount) const
		-> std::unique_ptr<WriterBase>;
	void writeCountSwitchDeclaration(WriterStream& stream) const;
	void writeOverload(WriterStream& writer,
	                   const view::FunctionOverload& target) const;
};

}  // namespace mapr::transform