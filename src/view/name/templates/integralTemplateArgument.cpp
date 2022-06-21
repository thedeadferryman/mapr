//
// Created by karl on 12.05.22.
//

#include <string>

#include "integralTemplateArgument.hpp"

#include <llvm/ADT/SmallString.h>

using mapr::view::IntegralTemplateArgument;

static constexpr auto DisplayRadix = 10U;
static constexpr auto BufferInitialLength = 16U;

IntegralTemplateArgument::IntegralTemplateArgument(llvm::APSInt value)
	: value(std::move(value)) {}

auto IntegralTemplateArgument::str() const -> std::string {
	auto buffer = llvm::SmallString<BufferInitialLength>();
	value.toString(buffer, DisplayRadix, value.isSigned());

	return std::string(buffer);
}
