//
// Created by karl on 22.03.22.
//

#include "commandLineException.hpp"

using mapr::app::CommandLineException;

CommandLineException::CommandLineException(std::string_view message)
	: BaseException(message) {}
