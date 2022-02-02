//
// Created by kmeinkopf on 07.01.2022.
//

#pragma once

#include "view/declBase.hpp"
#include "view/types/typeBase.hpp"

namespace kodgen::view {

class AliasType : public TypeBase {
	std::shared_ptr<TypeBase> source;
	std::string name;

  public:
	AliasType(const clang::Qualifiers& quals,
	          std::string_view name,
	          std::shared_ptr<TypeBase> source);

	[[nodiscard]] auto getSource() const -> const std::shared_ptr<TypeBase>&;

	[[nodiscard]] auto getQualifiedName() const -> const std::string&;

	[[nodiscard]] auto getPrettyName() const -> std::string override;
};

}  // namespace kodgen::view