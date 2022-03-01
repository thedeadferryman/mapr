//
// Created by kmeinkopf on 23.02.22.
//

#include "enumMapperFactory.hpp"

#include "transform/mappers/enum/enumMapper.hpp"

using kodgen::transform::EnumMapperFactory;

auto EnumMapperFactory::acceptsDecl(
	const std::shared_ptr<view::DeclBase>& decl) const -> bool {
	return std::dynamic_pointer_cast<view::EnumDecl>(decl) != nullptr;
}

auto EnumMapperFactory::mapperForDecl(
	std::shared_ptr<view::DeclBase> decl) const -> std::unique_ptr<MapperBase> {
	if (auto enumDecl = std::dynamic_pointer_cast<view::EnumDecl>(decl)) {
		return std::make_unique<EnumMapper>(enumDecl);
	}

	BOOST_ASSERT_MSG(false, "Unsupported Decl type");
}