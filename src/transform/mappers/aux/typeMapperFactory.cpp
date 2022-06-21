//
// Created by karl on 22.03.22.
//

#include "typeMapperFactory.hpp"

#include "transform/mappers/aux/typeMapper.hpp"

using mapr::transform::TypeMapperFactory;

TypeMapperFactory::TypeMapperFactory(
	std::shared_ptr<config::PipelineContext> context, MapperFactoryMode mode)
	: context(std::move(context))
	, mode(mode) {}

auto TypeMapperFactory::acceptsDecl(
	const std::shared_ptr<const view::DeclBase>& decl) const -> bool {
	return std::dynamic_pointer_cast<const TypeDecl>(decl) != nullptr
		&& decl->getDeclType() == view::DeclType::Type;
}
auto TypeMapperFactory::mapperForDecl(
	std::shared_ptr<const view::DeclBase> decl) const -> std::unique_ptr<MapperBase> {
	if (auto typeDecl = std::dynamic_pointer_cast<const TypeDecl>(decl)) {
		return std::make_unique<TypeMapper>(typeDecl, context, mode);
	}

	BOOST_ASSERT_MSG(false, "Unsupported Decl type");
}
