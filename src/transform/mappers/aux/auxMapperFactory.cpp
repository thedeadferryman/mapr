//
// Created by kmeinkopf on 23.02.22.
//

#include "auxMapperFactory.hpp"

#include "transform/aux/auxDecl.hpp"

#include "util/macro.hpp"

#include "auxMapper.hpp"

using kodgen::transform::AuxMapperFactory;

auto AuxMapperFactory::mapperForDecl(std::shared_ptr<view::DeclBase> decl) const
	-> std::unique_ptr<MapperBase> {
	if (auto auxDecl = std::dynamic_pointer_cast<AuxDecl>(decl)) {
		return std::make_unique<AuxMapper>(auxDecl);
	}

	BOOST_ASSERT_MSG(false, "Unsupported Decl type");
}
auto kodgen::transform::AuxMapperFactory::acceptsDecl(
	const std::shared_ptr<view::DeclBase>& decl) const -> bool {
	return std::dynamic_pointer_cast<AuxDecl>(decl) != nullptr;
}
