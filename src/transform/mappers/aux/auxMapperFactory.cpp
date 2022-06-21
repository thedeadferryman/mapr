//
// Created by kmeinkopf on 23.02.22.
//

#include "auxMapperFactory.hpp"

#include "transform/aux/auxDecl.hpp"

#include "transform/mappers/aux/auxMapper.hpp"

#include "util/macro.hpp"

using mapr::transform::AuxMapperFactory;

AuxMapperFactory::AuxMapperFactory(
	std::shared_ptr<config::PipelineContext> context)
	: context(std::move(context)) {}

auto AuxMapperFactory::mapperForDecl(std::shared_ptr<const view::DeclBase> decl)
	const -> std::unique_ptr<MapperBase> {
	if (auto auxDecl = std::dynamic_pointer_cast<const AuxDecl>(decl)) {
		return std::make_unique<AuxMapper>(auxDecl, context);
	}

	BOOST_ASSERT_MSG(false, "Unsupported Decl type");
}
auto AuxMapperFactory::acceptsDecl(
	const std::shared_ptr<const view::DeclBase>& decl) const -> bool {
	return std::dynamic_pointer_cast<const AuxDecl>(decl) != nullptr;
}
