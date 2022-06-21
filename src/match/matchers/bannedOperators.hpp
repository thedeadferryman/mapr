//
// Created by karl on 22.05.22.
//

#pragma once

#include <clang/Basic/OperatorKinds.h>

namespace mapr::match {

[[nodiscard]] auto isBannedOperator(clang::OverloadedOperatorKind kind) -> bool;

}  // namespace mapr::match