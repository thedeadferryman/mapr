//
// Created by kmeinkopf on 26.12.2021.
//

#pragma once

#include <unordered_map>

#include <boost/graph/adjacency_list.hpp>

#include "view/function/functionDecl.hpp"

#include "transform/mappers/mapperBase.hpp"
#include "transform/name/functionNameTransformer.hpp"
#include "transform/writers/writerBase.hpp"

#include "util/tree/nodeBase.hpp"

namespace kodgen::transform {

class FunctionMapper : public MapperBase {
	std::shared_ptr<view::FunctionDecl> functionDecl;
	std::shared_ptr<tree::NodeBase> overloadTree;
	std::unordered_map<std::size_t, std::size_t> argCounts;

  public:
	explicit FunctionMapper(std::shared_ptr<view::FunctionDecl> functionDecl);

	[[nodiscard]] auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;

	void writeDeclaration(WriterStream& writer) override;

	void writeDefinition(WriterStream& writer) override;

  private:
	[[nodiscard]] auto getSlugType(std::size_t argCount) const
		-> FunctionNameTransformer::SlugType;

	[[nodiscard]] auto typedOverloadDeclarationWriterI(
		std::shared_ptr<tree::NodeBase> node,
		kodgen::view::FunctionOverload path) const
		-> std::unique_ptr<WriterBase>;

	void typedOverloadDeclarationWriter(WriterStream& stream) const;

	void buildOverloadTree(std::shared_ptr<tree::NodeBase> node,
	                       const view::FunctionOverload& overload) const;

	void computeArgMap();
};

}  // namespace kodgen::getName