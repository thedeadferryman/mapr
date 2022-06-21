//
// Created by karl on 25.05.22.
//

#pragma once

#include <string>

#include <clang/Basic/SourceLocation.h>

#include "view/loc/sourceLoc.hpp"

namespace mapr::view {

class SingleSourceLoc : public SourceLoc {
  public:
	[[nodiscard]] static auto fromClangLoc(const clang::FullSourceLoc& location)
		-> std::shared_ptr<SingleSourceLoc>;

  private:
	std::string path;
	std::size_t line;
	std::size_t lineShift;
	std::size_t fileShift;

  public:
	explicit SingleSourceLoc(std::string_view path,
	                         size_t line = 0,
	                         size_t lineShift = 0,
	                         size_t fileShift = 0);

	[[nodiscard]] auto getPath() const -> const std::string&;

	[[nodiscard]] auto getLine() const -> size_t;

	[[nodiscard]] auto getLineShift() const -> size_t;
	[[nodiscard]] auto getFileShift() const -> size_t;

	[[nodiscard]] auto str() const -> std::string override;
};

}  // namespace mapr::view
