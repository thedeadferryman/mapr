cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    kodgen
    VERSION 0.0.1
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

docs_early_return()

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    kodgen_lib SHARED
    source/lib.cpp
)

target_include_directories(
    kodgen_lib ${kodgen_warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(kodgen_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

set(KODGEN_SOURCESET source/main.cpp)

add_executable(kodgen_kodgen ${KODGEN_SOURCESET})
add_executable(kodgen::kodgen ALIAS kodgen_kodgen)

set_target_properties(
    kodgen_kodgen PROPERTIES
    OUTPUT_NAME kodgen
    EXPORT_NAME kodgen
)

target_compile_features(kodgen_kodgen PRIVATE cxx_std_20)

target_link_libraries(kodgen_kodgen PRIVATE kodgen_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT kodgen_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of kodgen"
  )
endif()

include(cmake/dev-mode.cmake)
