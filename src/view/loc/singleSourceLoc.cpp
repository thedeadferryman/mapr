//
// Created by karl on 25.05.22.
//

#include "singleSourceLoc.hpp"

#include <clang/Basic/SourceManager.h>

#include "util/stringBuilder.hpp"

using mapr::view::SingleSourceLoc;

#define NULL_OR(_Value, _Fallback) \
	(((_Value) != nullptr) ? (_Value) : (_Fallback))

SingleSourceLoc::SingleSourceLoc(std::string_view path,
                                 size_t line,
                                 size_t lineShift,
                                 size_t fileShift)
	: path(path)
	, line(line)
	, lineShift(lineShift)
	, fileShift(fileShift) {}

auto SingleSourceLoc::getPath() const -> const std::string& {
	return path;
}

auto SingleSourceLoc::getLine() const -> size_t {
	return line;
}

auto SingleSourceLoc::getLineShift() const -> size_t {
	return lineShift;
}

auto SingleSourceLoc::getFileShift() const -> size_t {
	return fileShift;
}

auto SingleSourceLoc::fromClangLoc(const clang::FullSourceLoc& location)
	-> std::shared_ptr<SingleSourceLoc> {
	auto fileEntry =
		NULL_OR(location.getFileEntry(), location.getFileLoc().getFileEntry());

	auto pathname =
		(fileEntry == nullptr) ? "<internal>" : fileEntry->getName();

	return std::make_shared<SingleSourceLoc>(  //
		pathname,
		location.getLineNumber(),
		location.getColumnNumber(),
		location.getFileOffset());
}

auto SingleSourceLoc::str() const -> std::string {
	return util::StringBuilder(path) << ":" << line << ":" << lineShift;
}
