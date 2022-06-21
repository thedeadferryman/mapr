//
// Created by karl on 31.03.22.
//

#include "recordMapperFactory.hpp"

#include "view/record/recordDecl.hpp"

#include "transform/mappers/record/recordDeclarationMapper.hpp"
#include "transform/mappers/record/recordDefinitionMapper.hpp"

using mapr::transform::RecordMapperFactory;

RecordMapperFactory::RecordMapperFactory(
	MapperFactoryMode mode, std::shared_ptr<config::PipelineContext> context)
	: mode(mode)
	, context(std::move(context)) {}

auto RecordMapperFactory::acceptsDecl(
	const std::shared_ptr<const view::DeclBase>& decl) const -> bool {
	if (mode == MapperFactoryMode::Database) {
		return false;
	}

	return decl->getDeclType() == view::DeclType::Record;
}

auto RecordMapperFactory::mapperForDecl(
	std::shared_ptr<const view::DeclBase> decl) const -> std::unique_ptr<MapperBase> {
	if (auto recordDecl = std::dynamic_pointer_cast<const view::RecordDecl>(decl)) {
		switch (mode) {
			case MapperFactoryMode::Declaration:
				return std::make_unique<RecordDeclarationMapper>(recordDecl,
				                                                 context);
			case MapperFactoryMode::Definition:
				return std::make_unique<RecordDefinitionMapper>(recordDecl);
			default:;
		}
	}

	UNREACHABLE();
}
