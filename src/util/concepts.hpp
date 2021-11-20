//
// Created by kmeinkopf on 20.11.2021.
//

#pragma once

#include <concepts>

namespace kodgen::util {

template<typename T, typename P>
concept Convertible = std::convertible_to<T, P>;

}  // namespace kodgen::util