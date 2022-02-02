//
// Created by kmeinkopf on 13.01.2022.
//

#pragma once

#include "view/types/typeBase.hpp"

namespace kodgen::view {

class PointerType : public TypeBase {
	std::shared_ptr<TypeBase> pointee;

  public:
	PointerType(const clang::Qualifiers& quals,
	            std::shared_ptr<TypeBase> pointee);

	[[nodiscard]] auto getPointee() const -> const std::shared_ptr<TypeBase>&;

	[[nodiscard]] auto getPrettyName() const -> std::string override;
};

}  // namespace kodgen::view
