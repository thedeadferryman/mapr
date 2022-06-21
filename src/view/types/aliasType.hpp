//
// Created by kmeinkopf on 07.01.2022.
//

#pragma once

#include "view/declBase.hpp"
#include "view/name/qualifiedName.hpp"
#include "view/types/typeBase.hpp"

#include "view/types/pointerType.hpp"

namespace mapr::view {

class AliasType : public TypeBase {
	std::shared_ptr<TypeBase> source;
	QualifiedName name;

  public:
	AliasType(const clang::Qualifiers& quals,
	          QualifiedName qualName,
	          std::shared_ptr<TypeBase> source);

	[[nodiscard]] auto getSource() const -> const std::shared_ptr<TypeBase>&;

	[[nodiscard]] auto getName() const -> const QualifiedName&;

	[[nodiscard]] auto getPrettyName() const -> std::string override;
};

}  // namespace mapr::view