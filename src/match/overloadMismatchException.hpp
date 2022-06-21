//
// Created by karl on 31.03.22.
//

#pragma once

#include "view/declBase.hpp"

#include "util/exceptions/baseException.hpp"

namespace mapr::match {

class OverloadMismatchException : public util::BaseException {
  public:
	OverloadMismatchException(
		std::string_view name,
		const std::shared_ptr<const view::SourceLoc>& location,
		view::DeclType expectedType,
		view::DeclType actualType,
		const std::shared_ptr<const view::SourceLoc>& previousLocation);
};

}  // namespace mapr::match


