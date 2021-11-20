cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
        kodgen
        VERSION 0.0.1
        DESCRIPTION "C++-to-C bindings generator"
        LANGUAGES CXX
)

docs_early_return()

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Dependencies ----

find_package(Clang REQUIRED CONFIG)
find_package(Boost 1.70 REQUIRED COMPONENTS regex)

# ---- Declare library ----

set(LIBKODGEN_SOURCESET
        src/match/matcherBase.hpp
        src/match/matchContext.cpp src/match/matchContext.hpp
        src/match/matchers/primitiveFunctionMatcher.cpp src/match/matchers/primitiveFunctionMatcher.hpp
        src/view/type/typeBase.cpp src/view/type/typeBase.hpp src/match/matchers/internal/clangMatchers.hpp src/view/declBase.cpp src/view/declBase.hpp src/view/type/primitiveType.cpp src/view/type/primitiveType.hpp src/view/function/functionBase.hpp src/view/varDecl.hpp src/util/concepts.hpp src/view/function/primitiveFunction.cpp src/view/function/primitiveFunction.hpp)

add_library(kodgen-base SHARED ${LIBKODGEN_SOURCESET})

llvm_map_components_to_libnames(LLVM_LIBS support core)
target_link_libraries(kodgen-base PUBLIC clang-cpp LLVM)

target_include_directories(
        kodgen-base ${kodgen_warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
        ${LLVM_INCLUDE_DIRS}
        ${CLANG_INCLUDE_DIRS}
        ${BOOST_INCLUDE_DIRS}
)

target_compile_features(kodgen-base PUBLIC cxx_std_20)

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
target_compile_definitions(kodgen-base PUBLIC ${LLVM_DEFINITIONS_LIST})

# ---- Declare executable ----

set(KODGEN_SOURCESET src/main.cpp src/match/matchers/internal/clangMatchers.hpp)

add_executable(kodgen_kodgen ${KODGEN_SOURCESET})
add_executable(kodgen::kodgen ALIAS kodgen_kodgen)

set_target_properties(
        kodgen_kodgen PROPERTIES
        OUTPUT_NAME kodgen
        EXPORT_NAME kodgen
)

target_compile_features(kodgen_kodgen PRIVATE cxx_std_20)

target_link_libraries(kodgen_kodgen PRIVATE kodgen-base)

# ---- Install rules ----

if (NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/install-rules.cmake)
endif ()

# ---- Developer mode ----

if (NOT kodgen_DEVELOPER_MODE)
    return()
elseif (NOT PROJECT_IS_TOP_LEVEL)
    message(
            AUTHOR_WARNING
            "Developer mode is intended for developers of kodgen"
    )
endif ()

include(cmake/dev-mode.cmake)
