#include "lib.hpp"

auto main() -> int {
    library lib;

    return lib.varName == "kodgen" ? 0 : 1;
}
