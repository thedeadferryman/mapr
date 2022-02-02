//
// Created by kmeinkopf on 24.12.2021.
//

#include "alreadyExistsException.hpp"

#include "util/stringBuilder.hpp"

using kodgen::view::AlreadyExistsException;

AlreadyExistsException::AlreadyExistsException(std::string_view declId,
                                               view::DeclType type)
	: BaseException(std::string(util::StringBuilder()
                                << "'" << declId << "' of type '"
                                << view::stringOfDeclType(type)
                                << "' already exists!")) {}
