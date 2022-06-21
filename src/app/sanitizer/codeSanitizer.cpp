//
// Created by karl on 22.03.22.
//

#include <filesystem>

#include "codeSanitizer.hpp"

#include <clang/Format/Format.h>

using mapr::app::CodeSanitizer;

using clang::format::getStyle;
using llvm::StringRef;
namespace FS = std::filesystem;

CodeSanitizer::CodeSanitizer(config::FormatOptions options)
	: options(std::move(options)) {}

void CodeSanitizer::sanitize(std::string_view code, std::ostream& output) {
	auto formatFile = options.styleFile.has_value()
		? FS::current_path() / options.styleFile.value()
		: "";

	auto maybeStyle = getStyle(  //
		options.formatStyle,
		StringRef(formatFile),
		options.fallbackStyle,
		code,
		nullptr,
		options.allowUnknownOptions  //
	);

	[[unlikely]] if (!maybeStyle) {
		llvm::errs() << "Formatting failed: "
					 << errorToErrorCode(maybeStyle.takeError()).message()
					 << "\n";

		output << code;

		return;
	}

	auto range = clang::tooling::Range(0, code.length());
	auto replacements =
		clang::format::reformat(maybeStyle.get(), code, llvm::ArrayRef(range));
	auto maybeCode = clang::tooling::applyAllReplacements(code, replacements);

	[[likely]] if (maybeCode) {
		output << maybeCode.get();

		return;
	}

	llvm::errs() << "Formatting failed: "
				 << errorToErrorCode(maybeCode.takeError()).message() << "\n";

	output << code;
}
