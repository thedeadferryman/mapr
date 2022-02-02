//
// Created by kmeinkopf on 20.01.2022.
//

#include "resourceNotFoundException.hpp"

#include "util/stringBuilder.hpp"

using kodgen::util::ResourceNotFoundException;

ResourceNotFoundException::ResourceNotFoundException(
	kodgen::util::ResourceCategory category, std::string_view name)
	: BaseException(std::string(StringBuilder()
                                << "Resource '" << name << "' of type "
                                << stringOfResourceCategory(category)
                                << " is missing!")) {}
