//
// Created by karl on 25.05.22.
//

#pragma once

#include "view/loc/sparseSourceLoc.hpp"

#include "view/declBase.hpp"

namespace mapr::view {

class SparseDecl : public DeclBase {
  public:
	SparseDecl(std::string_view id, DeclType kind);

	[[nodiscard]] auto getLocation() const
		-> std::shared_ptr<SourceLoc> override;

	[[nodiscard]] virtual auto getSparseLocation() const
		-> std::shared_ptr<SparseSourceLoc> = 0;
};

}  // namespace mapr::view
