//
// Created by kmeinkopf on 23.02.22.
//

#include <utility>

#include "functionMapperFactory.hpp"

#include "view/record/recordMethodDecl.hpp"

#include "transform/mappers/function/functionDeclarationMapper.hpp"
#include "transform/mappers/function/functionDefinitionMapper.hpp"

#include "util/macro.hpp"

#include "config/pipelineContext.hpp"
#include "config/toolProperties.hpp"

using mapr::transform::FunctionMapperFactory;

FunctionMapperFactory::FunctionMapperFactory(
	MapperFactoryMode mode, std::shared_ptr<config::PipelineContext> context)
	: mode(mode)
	, context(std::move(context)) {}

auto FunctionMapperFactory::acceptsDecl(
	const std::shared_ptr<const view::DeclBase>& decl) const -> bool {
	auto isFunc =
		std::dynamic_pointer_cast<const view::FunctionDecl>(decl) != nullptr;

	if (mode == MapperFactoryMode::Definition) {
		return isFunc
			&& (  //
				   decl->getDeclType() == view::DeclType::Function  //
				   || decl->getDeclType()
					   == view::DeclType::OperatorOverload);
	}

	return isFunc;
}

auto FunctionMapperFactory::mapperForDecl(
	std::shared_ptr<const view::DeclBase> decl) const -> std::unique_ptr<MapperBase> {
	if (auto funcDecl = std::dynamic_pointer_cast<const view::FunctionDecl>(decl)) {
		switch (mode) {
			case MapperFactoryMode::Declaration:
				return std::make_unique<FunctionDeclarationMapper>(
					std::move(funcDecl), context);
				;
			case MapperFactoryMode::Definition:
				return std::make_unique<FunctionDefinitionMapper>(
					std::move(funcDecl), context);
			default:
				return nullptr;
		}
	}

	BOOST_ASSERT_MSG(false, "Unsupported Decl type");
}
