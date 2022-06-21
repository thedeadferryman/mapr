//
// Created by karl on 31.03.22.
//

#include "overloadMismatchException.hpp"

#include "util/stringBuilder.hpp"

using mapr::match::OverloadMismatchException;
using mapr::util::StringBuilder;

OverloadMismatchException::OverloadMismatchException(
	std::string_view name,
	const std::shared_ptr<const view::SourceLoc>& location,
	view::DeclType expectedType,
	view::DeclType actualType,
	const std::shared_ptr<const view::SourceLoc>& previousLocation)
	: BaseException(std::string(
		StringBuilder("wrong overload: '")
		<< name << "' declared as '" << view::stringOfDeclType(actualType)
		<< "' at " << location->str() << ", previously declared as '"
		<< view::stringOfDeclType(expectedType) << "' at "
		<< previousLocation->str() << "!")) {}