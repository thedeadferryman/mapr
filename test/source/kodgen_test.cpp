#include "lib.hpp"

auto main() -> int {
    library lib;

    return lib.name == "kodgen" ? 0 : 1;
}
