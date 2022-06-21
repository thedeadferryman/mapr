//
// Created by kmeinkopf on 14.03.22.
//

#pragma once

#include "view/record/recordDecl.hpp"
#include "view/types/pointerType.hpp"
#include "view/types/typeBase.hpp"

namespace mapr::view {

BIT_ENUM(ConstMode, None, ConstPointer, ConstValue);

class RecordType : public TypeBase {
  public:
	static auto makePointer(const QualifiedName& qualifiedName,
	                        ConstMode constMode = ConstMode::None)
		-> std::shared_ptr<PointerType>;

  private:
	QualifiedName qualifiedName;

  public:
	RecordType(const clang::Qualifiers& qualifiers, QualifiedName nsName);

	[[nodiscard]] auto getPrettyName() const -> std::string override;

	[[nodiscard]] auto getName() const -> const QualifiedName&;

	[[nodiscard]] auto asPointer(ConstMode constMode = ConstMode::None) const -> std::shared_ptr<PointerType>;
};

}  // namespace mapr::view