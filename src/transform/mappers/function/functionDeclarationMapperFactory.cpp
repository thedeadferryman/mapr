//
// Created by kmeinkopf on 23.02.22.
//

#include "functionDeclarationMapperFactory.hpp"

#include "transform/mappers/function/functionDeclarationMapper.hpp"

#include "util/macro.hpp"

using kodgen::transform::FunctionDeclarationMapperFactory;

FunctionDeclarationMapperFactory::FunctionDeclarationMapperFactory(
	MapperFactoryMode mode)
	: MapperFactoryBase()
	, mode(mode) {}

auto FunctionDeclarationMapperFactory::acceptsDecl(
	const std::shared_ptr<view::DeclBase>& decl) const -> bool {
	return std::dynamic_pointer_cast<view::FunctionDecl>(decl) != nullptr;
}

auto FunctionDeclarationMapperFactory::mapperForDecl(
	std::shared_ptr<view::DeclBase> decl) const -> std::unique_ptr<MapperBase> {
	if (auto funcDecl = std::dynamic_pointer_cast<view::FunctionDecl>(decl)) {
		return std::make_unique<FunctionDeclarationMapper>(std::move(funcDecl));
	}

	BOOST_ASSERT_MSG(false, "Unsupported Decl type");
}
