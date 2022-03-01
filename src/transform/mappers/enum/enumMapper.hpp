//
// Created by kmeinkopf on 02.02.2022.
//

#pragma once

#include "view/enum/enumDecl.hpp"

#include "transform/mappers/base/mapperBase.hpp"

namespace kodgen::transform {

class EnumMapper : public MapperBase {
	std::shared_ptr<view::EnumDecl> enumDecl;

  public:
	EnumMapper(std::shared_ptr<view::EnumDecl> enumDecl);

	auto checkDependencies() const
		-> std::vector<std::shared_ptr<DependencyRequest>> override;

	void write(WriterStream& writer) override;

  private:
	[[nodiscard]] auto
	getMemberName(  // TODO: think of extracting this logic to a separate module
		const std::shared_ptr<view::EnumMemberDecl>& member) const -> std::string;
};

}  // namespace kodgen::transform