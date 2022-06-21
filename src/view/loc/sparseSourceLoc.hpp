//
// Created by karl on 25.05.22.
//

#pragma once

#include "view/loc/singleSourceLoc.hpp"
#include "view/loc/sourceLoc.hpp"
namespace mapr::view {

class SparseSourceLoc : public SourceLoc {
	std::vector<std::shared_ptr<SingleSourceLoc>> locations;

  public:
	SparseSourceLoc();

	explicit SparseSourceLoc(std::vector<std::shared_ptr<SingleSourceLoc>> locations);

	[[nodiscard]] auto getLocations() const
		-> const std::vector<std::shared_ptr<SingleSourceLoc>>&;

	[[nodiscard]] auto isEmpty() const -> bool;

	[[nodiscard]] auto str() const -> std::string override;
};

}  // namespace mapr::view
