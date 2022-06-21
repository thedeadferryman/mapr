//
// Created by kmeinkopf on 24.12.2021.
//

#include "baseException.hpp"

using mapr::util::BaseException;

BaseException::BaseException(std::string_view message)
	: std::runtime_error(std::string(message)) {}
