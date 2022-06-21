//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include <memory>
#include <unordered_map>

#include "view/declBase.hpp"
#include "view/function/functionDecl.hpp"

#include "transform/dependencies/dependencyResolver.hpp"
#include "transform/writerStream.hpp"

#include "transform/mappers/base/mapperBase.hpp"
#include "transform/mappers/base/mapperFactoryBase.hpp"

#include "config/toolProperties.hpp"

namespace mapr::transform {

class SourceTransformer {
	WriterStream& writerStream;
	DependencyResolver dependencyResolver;
	std::unordered_map<std::string, bool> writtenDecls;
	std::vector<std::shared_ptr<MapperFactoryBase>> factories;

  public:
	explicit SourceTransformer(WriterStream& wStream,
	                           DependencyResolver declResolver);

	void writeDecl(const std::shared_ptr<const view::DeclBase>& decl,
	               std::string_view pipelineName = "<anonymous>");

	auto resolveDependency(std::shared_ptr<DependencyRequest> dependencyId)
		const -> std::shared_ptr<const view::DeclBase>;

	void registerMapper(std::shared_ptr<MapperFactoryBase> factoryBase);

  private:
	auto mapperForDecl(std::shared_ptr<const view::DeclBase> decl)
		-> std::unique_ptr<MapperBase>;
	void writeDependency(const std::shared_ptr<DependencyRequest>& request,
	                     std::string_view& pipelineName);
};

}  // namespace mapr::transform
