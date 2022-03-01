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
find_package(Boost 1.70 REQUIRED COMPONENTS regex graph)

find_package(nlohmann_json 3.10.4 REQUIRED)
find_package(fmt 8.0.1 REQUIRED)

# ---- Project libraries ----

set(LIBKODGEN_SOURCESET
        src/match/matcherBase.hpp
        src/view/declContext.cpp src/view/declContext.hpp
        src/match/matchers/functionMatcher.cpp src/match/matchers/functionMatcher.hpp
        src/match/matchers/internal/clangMatchers.hpp
        src/view/declBase.cpp src/view/declBase.hpp
        src/view/function/functionOverload.hpp src/view/function/functionOverload.cpp
        src/view/varDecl.hpp src/view/varDecl.cpp
        src/match/matchers/structMatcher.cpp src/match/matchers/structMatcher.hpp
        src/view/exceptions/alreadyExistsException.cpp src/view/exceptions/alreadyExistsException.hpp
        src/util/exceptions/baseException.cpp src/util/exceptions/baseException.hpp
        src/view/namespacedName.cpp src/view/namespacedName.hpp
        src/match/nameResolver.cpp src/match/nameResolver.hpp
        src/util/macro.hpp
        src/transform/mappers/base/mapperBase.hpp
        src/view/function/functionDecl.cpp src/view/function/functionDecl.hpp
        src/match/matchers/typedefMatcher.cpp src/match/matchers/typedefMatcher.hpp
        src/view/types/aliasType.cpp src/view/types/aliasType.hpp
        src/match/matchers/enumMatcher.cpp src/match/matchers/enumMatcher.hpp
        src/view/enum/enumDecl.cpp src/view/enum/enumDecl.hpp
        src/view/enum/enumMemberDecl.cpp src/view/enum/enumMemberDecl.hpp
        src/view/types/typeBase.cpp src/view/types/typeBase.hpp
        src/view/types/builtinType.cpp src/view/types/builtinType.hpp
        src/view/types/pointerType.cpp src/view/types/pointerType.hpp
        src/view/types/referenceType.cpp src/view/types/referenceType.hpp
        src/util/stringBuilder.cpp src/util/stringBuilder.hpp
        src/view/types/enumType.cpp src/view/types/enumType.hpp
        src/match/typeResolver.cpp src/match/typeResolver.hpp
        src/match/matchers/internal/clangMatchers.hpp
        src/transform/mappers/function/functionDeclarationMapper.cpp src/transform/mappers/function/functionDeclarationMapper.hpp
        src/transform/writerStream.cpp src/transform/writerStream.hpp
        src/transform/writers/writerBase.cpp src/transform/writers/writerBase.hpp
        src/transform/sourceTransformer.cpp src/transform/sourceTransformer.hpp
        src/transform/exceptions/undefinedDeclException.cpp src/transform/exceptions/undefinedDeclException.hpp
        src/transform/dependencies/dependencyResolver.cpp src/transform/dependencies/dependencyResolver.hpp
        src/transform/writers/functionWriter.cpp src/transform/writers/functionWriter.hpp
        src/transform/writers/externCWriter.cpp src/transform/writers/externCWriter.hpp
        src/util/matchType.hpp
        src/transform/name/functionNameTransformer.cpp src/transform/name/functionNameTransformer.hpp
        src/view/types.hpp
        src/util/stringBuilderSupportedTypes.inc
        src/transform/name/typeNameTransformer.cpp
        src/transform/name/typeNameTransformer.hpp
        src/transform/exceptions/unsupportedTypeException.cpp src/transform/exceptions/unsupportedTypeException.hpp
        src/transform/aux/auxDecl.cpp src/transform/aux/auxDecl.hpp
        src/transform/aux/templateFile.cpp src/transform/aux/templateFile.hpp
        src/util/resourceLoader.cpp src/util/resourceLoader.hpp
        src/util/exceptions/resourceNotFoundException.cpp src/util/exceptions/resourceNotFoundException.hpp
        src/transform/mappers/aux/auxMapper.cpp src/transform/mappers/aux/auxMapper.hpp
        src/transform/writers/textWriter.cpp src/transform/writers/textWriter.hpp
        src/transform/dependencies/dependencyRequest.cpp src/transform/dependencies/dependencyRequest.hpp
        src/transform/dependencies/declRequest.cpp src/transform/dependencies/declRequest.hpp src/transform/dependencies/auxRequest.cpp src/transform/dependencies/auxRequest.hpp src/transform/dependencies/typeRequest.cpp src/transform/dependencies/typeRequest.hpp src/transform/aux/typeDecl.cpp src/transform/aux/typeDecl.hpp src/transform/mappers/typeMapper.cpp src/transform/mappers/typeMapper.hpp src/transform/writers/includeWriter.cpp src/transform/writers/includeWriter.hpp src/transform/writers/defineConditionWriter.cpp src/transform/writers/defineConditionWriter.hpp src/transform/writers/c11GenericWriter.cpp src/transform/writers/c11GenericWriter.hpp src/util/tree/nodeBase.cpp src/util/tree/nodeBase.hpp src/util/tree/valueNode.cpp src/util/tree/valueNode.hpp src/transform/writers/defineWriter.cpp src/transform/writers/defineWriter.hpp src/transform/writers/lineEscapedWriter.cpp src/transform/writers/lineEscapedWriter.hpp src/transform/writers/ieeWriter.cpp src/transform/writers/ieeWriter.hpp src/transform/mappers/enum/enumMapper.cpp src/transform/mappers/enum/enumMapper.hpp src/transform/writers/blockWriter.cpp src/transform/writers/blockWriter.hpp src/transform/writers/typedefWriter.cpp src/transform/writers/typedefWriter.hpp src/transform/name/namespacedNameTransformer.cpp src/transform/name/namespacedNameTransformer.hpp src/transform/writers/enumWriter.cpp src/transform/writers/enumWriter.hpp src/transform/writers/sequentialWriter.cpp src/transform/writers/sequentialWriter.hpp src/util/collectionUtils.cpp src/util/collectionUtils.hpp src/transform/mappers/base/mapperFactoryBase.hpp src/transform/mappers/aux/auxMapperFactory.cpp src/transform/mappers/aux/auxMapperFactory.hpp src/transform/mappers/base/mapperFactoryBase.cpp src/transform/mappers/function/functionDeclarationMapperFactory.cpp src/transform/mappers/function/functionDeclarationMapperFactory.hpp src/transform/mappers/enum/enumMapperFactory.cpp src/transform/mappers/enum/enumMapperFactory.hpp src/match/matchers/classMatcher.cpp src/match/matchers/classMatcher.hpp src/transform/writers/expressionCallWriter.cpp src/transform/writers/expressionCallWriter.hpp)

add_library(kodgen-base SHARED ${LIBKODGEN_SOURCESET})

llvm_map_components_to_libnames(LLVM_LIBS support core)
target_link_libraries(kodgen-base PUBLIC clang-cpp LLVM fmt::fmt)

target_include_directories(
        kodgen-base ${kodgen_warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
        ${LLVM_INCLUDE_DIRS}
        ${CLANG_INCLUDE_DIRS}
        ${BOOST_INCLUDE_DIRS}
)

target_compile_features(kodgen-base PUBLIC cxx_std_20)

if (CMAKE_BUILD_TYPE STREQUAL Debug AND CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
target_compile_definitions(kodgen-base PUBLIC ${LLVM_DEFINITIONS_LIST})

# ---- Declare executable ----

set(KODGEN_SOURCESET src/app/main.cpp)

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
