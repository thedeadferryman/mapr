//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include <memory>
#include <unordered_map>

#include "view/declBase.hpp"
#include "view/function/functionDecl.hpp"

#include "transform/dependencies/dependencyResolver.hpp"
#include "transform/mappers/mapperBase.hpp"
#include "transform/writerStream.hpp"

namespace kodgen::transform {

class SourceTransformer {
	WriterStream& writerStream;
	DependencyResolver dependencyResolver;
	std::unordered_map<std::string, bool> writtenDecls;

  public:
	explicit SourceTransformer(WriterStream& wStream,
	                           DependencyResolver declResolver);

	void writeDecl(std::shared_ptr<view::DeclBase> decl);

	auto resolveDependency(std::shared_ptr<DependencyRequest> dependencyId) const
		-> std::shared_ptr<view::DeclBase>;

  private:
	static auto mapperForDecl(std::shared_ptr<view::DeclBase>)
		-> std::unique_ptr<MapperBase>;
};

}  // namespace kodgen::getName
