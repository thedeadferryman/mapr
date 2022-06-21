//
// Created by karl on 25.05.22.
//

#pragma once

#include <memory>

namespace mapr::view {

class SourceLoc {
  public:
	[[nodiscard]] virtual auto str() const -> std::string = 0;

	SourceLoc() = default;
	SourceLoc(const SourceLoc&) = delete;
	SourceLoc(SourceLoc&&) = delete;
	auto operator=(const SourceLoc&) -> SourceLoc& = delete;
	auto operator=(SourceLoc&&) -> SourceLoc& = delete;
	virtual ~SourceLoc() = default;
};

}  // namespace mapr::view
