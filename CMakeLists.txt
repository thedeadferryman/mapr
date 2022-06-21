cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
        mapr
        VERSION 0.1.1
        DESCRIPTION "C++-to-C bindings generator"
        LANGUAGES CXX

)

docs_early_return()

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Dependencies ----

find_package(Clang REQUIRED)
find_package(Boost 1.70 REQUIRED)

find_package(fmt 8.0.1 REQUIRED)
find_package(yaml-cpp 0.7.0 REQUIRED)

# ---- Project libraries ----

# region LIBMAPR_config_SOURCESET
set(LIBMAPR_config_SOURCESET
        src/config/configParser.cpp
        src/config/configParser.hpp
        src/config/invalidConfigException.cpp
        src/config/invalidConfigException.hpp
        src/config/pipelineContext.cpp
        src/config/pipelineContext.hpp
        src/config/toolProperties.cpp
        src/config/toolProperties.hpp
        )
# endregion

# region LIBMAPR_filtering_SOURCESET
set(LIBMAPR_filtering_SOURCESET
        src/filtering/filterChain.cpp
        src/filtering/filterChain.hpp
        src/filtering/filters/declNameFilter.cpp
        src/filtering/filters/declNameFilter.hpp
        src/filtering/filters/filenameFilter.cpp
        src/filtering/filters/filenameFilter.hpp
        src/filtering/filters/filterBase.cpp
        src/filtering/filters/filterBase.hpp
        )
# endregion

# region LIBMAPR_match_SOURCESET
set(LIBMAPR_match_SOURCESET
        src/match/matchers/bannedOperators.cpp
        src/match/matchers/bannedOperators.hpp
        src/match/matchers/enumMatcher.cpp
        src/match/matchers/enumMatcher.hpp
        src/match/matchers/functionMatcher.cpp
        src/match/matchers/functionMatcher.hpp
        src/match/matchers/internal/clangMatchers.hpp
        src/match/matchers/internal/clangMatchers.hpp
        src/match/matchers/internal/functionHelpers.cpp
        src/match/matchers/internal/functionHelpers.hpp
        src/match/matchers/internal/recordHandler.cpp
        src/match/matchers/internal/recordHandler.hpp
        src/match/matchers/matcherBase.hpp
        src/match/matchers/operatorMatcher.cpp
        src/match/matchers/operatorMatcher.hpp
        src/match/matchers/recordMatcher.cpp
        src/match/matchers/recordMatcher.hpp
        src/match/matchers/templateInstantiationMatcher.cpp
        src/match/matchers/templateInstantiationMatcher.hpp
        src/match/matchers/typedefMatcher.cpp
        src/match/matchers/typedefMatcher.hpp
        src/match/nameResolver.cpp
        src/match/nameResolver.hpp
        src/match/overloadMismatchException.cpp
        src/match/overloadMismatchException.hpp
        src/match/templateArgumentResolver.cpp
        src/match/templateArgumentResolver.hpp
        src/match/typeResolver.cpp
        src/match/typeResolver.hpp
        )
# endregion

# region LIBMAPR_transform_SOURCESET
set(LIBMAPR_transform_SOURCESET
        src/transform/aux/auxDecl.cpp
        src/transform/aux/auxDecl.hpp
        src/transform/aux/includeFile.cpp
        src/transform/aux/includeFile.hpp
        src/transform/aux/templateFile.cpp
        src/transform/aux/templateFile.hpp
        src/transform/aux/typeDecl.cpp
        src/transform/aux/typeDecl.hpp
        src/transform/dependencies/auxRequest.cpp
        src/transform/dependencies/auxRequest.hpp
        src/transform/dependencies/declRequest.cpp
        src/transform/dependencies/declRequest.hpp
        src/transform/dependencies/dependencyRequest.cpp
        src/transform/dependencies/dependencyRequest.hpp
        src/transform/dependencies/dependencyResolver.cpp
        src/transform/dependencies/dependencyResolver.hpp
        src/transform/dependencies/typeRequest.cpp
        src/transform/dependencies/typeRequest.hpp
        src/transform/exceptions/undefinedDeclException.cpp
        src/transform/exceptions/undefinedDeclException.hpp
        src/transform/exceptions/unsupportedTypeException.cpp
        src/transform/exceptions/unsupportedTypeException.hpp
        src/transform/mappers/aux/auxMapper.cpp
        src/transform/mappers/aux/auxMapper.hpp
        src/transform/mappers/aux/auxMapperFactory.cpp
        src/transform/mappers/aux/auxMapperFactory.hpp
        src/transform/mappers/aux/typeMapper.cpp
        src/transform/mappers/aux/typeMapper.hpp
        src/transform/mappers/aux/typeMapperFactory.cpp
        src/transform/mappers/aux/typeMapperFactory.hpp
        src/transform/mappers/base/mapperBase.hpp
        src/transform/mappers/base/mapperFactoryBase.cpp
        src/transform/mappers/base/mapperFactoryBase.hpp
        src/transform/mappers/common/functionLikeDefinitionMapper.cpp
        src/transform/mappers/common/functionLikeDefinitionMapper.hpp
        src/transform/mappers/enum/enumMapper.cpp
        src/transform/mappers/enum/enumMapper.hpp
        src/transform/mappers/enum/enumMapperFactory.cpp
        src/transform/mappers/enum/enumMapperFactory.hpp
        src/transform/mappers/function/functionDeclarationMapper.cpp
        src/transform/mappers/function/functionDeclarationMapper.hpp
        src/transform/mappers/function/functionDefinitionMapper.cpp
        src/transform/mappers/function/functionDefinitionMapper.hpp
        src/transform/mappers/function/functionMapperFactory.cpp
        src/transform/mappers/function/functionMapperFactory.hpp
        src/transform/mappers/record/recordCtorDefinitionMapper.cpp
        src/transform/mappers/record/recordCtorDefinitionMapper.hpp
        src/transform/mappers/record/recordDeclarationMapper.cpp
        src/transform/mappers/record/recordDeclarationMapper.hpp
        src/transform/mappers/record/recordDefinitionMapper.cpp
        src/transform/mappers/record/recordDefinitionMapper.hpp
        src/transform/mappers/record/recordDtorDefinitionMapper.cpp
        src/transform/mappers/record/recordDtorDefinitionMapper.hpp
        src/transform/mappers/record/recordMapperFactory.cpp
        src/transform/mappers/record/recordMapperFactory.hpp
        src/transform/mappers/record/recordMethodDefinitionMapper.cpp
        src/transform/mappers/record/recordMethodDefinitionMapper.hpp
        src/transform/mappers/record/recordMethodsMapperFactory.cpp
        src/transform/mappers/record/recordMethodsMapperFactory.hpp
        src/transform/mappers/record/syntheticGetterDefinitionMapper.cpp
        src/transform/mappers/record/syntheticGetterDefinitionMapper.hpp
        src/transform/mappers/record/syntheticSetterDefinitionMapper.cpp
        src/transform/mappers/record/syntheticSetterDefinitionMapper.hpp
        src/transform/name/functionNameTransformer.cpp
        src/transform/name/functionNameTransformer.hpp
        src/transform/name/qualifiedNameTransformer.cpp
        src/transform/name/qualifiedNameTransformer.hpp
        src/transform/name/typeNameTransformer.cpp
        src/transform/name/typeNameTransformer.hpp
        src/transform/sourceTransformer.cpp
        src/transform/sourceTransformer.hpp
        src/transform/typecastHelper.cpp
        src/transform/typecastHelper.hpp
        src/transform/writers/assignmentWriter.cpp
        src/transform/writers/assignmentWriter.hpp
        src/transform/writers/blockWriter.cpp
        src/transform/writers/blockWriter.hpp
        src/transform/writers/c11GenericWriter.cpp
        src/transform/writers/c11GenericWriter.hpp
        src/transform/writers/cxxCastWriter.cpp
        src/transform/writers/cxxCastWriter.hpp
        src/transform/writers/defineConditionWriter.cpp
        src/transform/writers/defineConditionWriter.hpp
        src/transform/writers/defineWriter.cpp
        src/transform/writers/defineWriter.hpp
        src/transform/writers/deleteWriter.cpp
        src/transform/writers/deleteWriter.hpp
        src/transform/writers/dereferenceWriter.cpp
        src/transform/writers/dereferenceWriter.hpp
        src/transform/writers/dynCtorInvocationWriter.cpp
        src/transform/writers/dynCtorInvocationWriter.hpp
        src/transform/writers/enumWriter.cpp
        src/transform/writers/enumWriter.hpp
        src/transform/writers/expressionCallWriter.cpp
        src/transform/writers/expressionCallWriter.hpp
        src/transform/writers/externCWriter.cpp
        src/transform/writers/externCWriter.hpp
        src/transform/writers/function/functionHeadWriter.cpp
        src/transform/writers/function/functionHeadWriter.hpp
        src/transform/writers/function/functionInvocationWriter.cpp
        src/transform/writers/function/functionInvocationWriter.hpp
        src/transform/writers/ieeWriter.cpp
        src/transform/writers/ieeWriter.hpp
        src/transform/writers/includeWriter.cpp
        src/transform/writers/includeWriter.hpp
        src/transform/writers/lineEscapedWriter.cpp
        src/transform/writers/lineEscapedWriter.hpp
        src/transform/writers/macroBlockWriter.cpp
        src/transform/writers/macroBlockWriter.hpp
        src/transform/writers/memberAccessWriter.cpp
        src/transform/writers/memberAccessWriter.hpp
        src/transform/writers/pointerizeWriter.cpp
        src/transform/writers/pointerizeWriter.hpp
        src/transform/writers/pointerTypeWriter.cpp
        src/transform/writers/pointerTypeWriter.hpp
        src/transform/writers/sequentialWriter.cpp
        src/transform/writers/sequentialWriter.hpp
        src/transform/writers/statement/plainStatementWriter.cpp
        src/transform/writers/statement/plainStatementWriter.hpp
        src/transform/writers/statement/returnStatementWriter.cpp
        src/transform/writers/statement/returnStatementWriter.hpp
        src/transform/writers/textWriter.cpp
        src/transform/writers/textWriter.hpp
        src/transform/writers/typedefWriter.cpp
        src/transform/writers/typedefWriter.hpp
        src/transform/writers/writerBase.cpp
        src/transform/writers/writerBase.hpp
        src/transform/writerStream.cpp
        src/transform/writerStream.hpp
        )
# endregion

# region LIBMAPR_util_SOURCESET
set(LIBMAPR_util_SOURCESET
        src/util/collectionUtils.cpp
        src/util/collectionUtils.hpp
        src/util/exceptions/baseException.cpp
        src/util/exceptions/baseException.hpp
        src/util/exceptions/resourceNotFoundException.cpp
        src/util/exceptions/resourceNotFoundException.hpp
        src/util/macro.hpp
        src/util/matchType.hpp
        src/util/resourceLoader.cpp
        src/util/resourceLoader.hpp
        src/util/stringBuilder.cpp
        src/util/stringBuilder.hpp
        src/util/tree/nodeBase.cpp
        src/util/tree/nodeBase.hpp
        src/util/tree/valueNode.cpp
        src/util/tree/valueNode.hpp
        )
# endregion

# region LIBMAPR_view_SOURCESET
set(LIBMAPR_view_SOURCESET
        src/view/declBase.cpp
        src/view/declBase.hpp
        src/view/declContext.cpp src/view/declContext.hpp
        src/view/enum/enumDecl.cpp
        src/view/enum/enumDecl.hpp
        src/view/enum/enumMemberDecl.cpp
        src/view/enum/enumMemberDecl.hpp
        src/view/exceptions/alreadyExistsException.cpp
        src/view/exceptions/alreadyExistsException.hpp
        src/view/function/functionDecl.cpp
        src/view/function/functionDecl.hpp
        src/view/function/functionOverload.cpp
        src/view/function/functionOverload.hpp
        src/view/function/operatorDecl.cpp
        src/view/function/operatorDecl.hpp
        src/view/loc/singleSourceLoc.cpp
        src/view/loc/singleSourceLoc.hpp
        src/view/loc/sourceLoc.hpp
        src/view/loc/sparseSourceLoc.cpp
        src/view/loc/sparseSourceLoc.hpp
        src/view/name/nameFragment.cpp
        src/view/name/nameFragment.hpp
        src/view/name/qualifiedName.cpp
        src/view/name/qualifiedName.hpp
        src/view/name/templates/integralTemplateArgument.cpp
        src/view/name/templates/integralTemplateArgument.hpp
        src/view/name/templates/nullPtrTemplateArgument.cpp
        src/view/name/templates/nullPtrTemplateArgument.hpp
        src/view/name/templates/nullTemplateArgument.cpp
        src/view/name/templates/nullTemplateArgument.hpp
        src/view/name/templates/templateArgumentBase.cpp
        src/view/name/templates/templateArgumentBase.hpp
        src/view/name/templates/typeTemplateArgument.cpp
        src/view/name/templates/typeTemplateArgument.hpp
        src/view/record/recordCtorDecl.cpp
        src/view/record/recordCtorDecl.hpp
        src/view/record/recordDecl.cpp
        src/view/record/recordDecl.hpp
        src/view/record/recordDtorDecl.cpp
        src/view/record/recordDtorDecl.hpp
        src/view/record/recordMethodDecl.cpp
        src/view/record/recordMethodDecl.hpp
        src/view/record/recordOperatorDecl.cpp
        src/view/record/recordOperatorDecl.hpp
        src/view/record/syntheticGetterDecl.cpp
        src/view/record/syntheticGetterDecl.hpp
        src/view/record/syntheticSetterDecl.cpp
        src/view/record/syntheticSetterDecl.hpp
        src/view/sparseDecl.cpp
        src/view/sparseDecl.hpp
        src/view/types.hpp
        src/view/types/aliasType.cpp
        src/view/types/aliasType.hpp
        src/view/types/builtinType.cpp
        src/view/types/builtinType.hpp
        src/view/types/enumType.cpp
        src/view/types/enumType.hpp
        src/view/types/pointerType.cpp
        src/view/types/pointerType.hpp
        src/view/types/recordType.cpp
        src/view/types/recordType.hpp
        src/view/types/referenceType.cpp
        src/view/types/referenceType.hpp
        src/view/types/typeBase.cpp
        src/view/types/typeBase.hpp
        src/view/varDecl.cpp
        src/view/varDecl.hpp
        )
# endregion

llvm_map_components_to_libnames(LLVM_LIBS support core)

add_library(mapr-commons INTERFACE)

add_library(mapr-config SHARED ${LIBMAPR_config_SOURCESET})
add_library(mapr-match SHARED ${LIBMAPR_match_SOURCESET})
add_library(mapr-transform SHARED ${LIBMAPR_transform_SOURCESET})
add_library(mapr-util SHARED ${LIBMAPR_util_SOURCESET})
add_library(mapr-view SHARED ${LIBMAPR_view_SOURCESET} ${LIBMAPR_filtering_SOURCESET})

target_include_directories(mapr-commons ${mapr_warning_guard} INTERFACE
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
        ${LLVM_INCLUDE_DIRS}
        ${CLANG_INCLUDE_DIRS}
        ${BOOST_INCLUDE_DIRS})

target_compile_features(mapr-commons INTERFACE cxx_std_20)

target_link_libraries(mapr-config PUBLIC mapr-commons ${YAML_CPP_LIBRARIES} mapr-util)
target_link_libraries(mapr-match PUBLIC mapr-commons LLVM ${LLVM_LIBS} clang-cpp mapr-view mapr-util)
target_link_libraries(mapr-transform PUBLIC mapr-commons LLVM ${LLVM_LIBS} clang-cpp fmt::fmt mapr-config mapr-view mapr-util)
target_link_libraries(mapr-view PUBLIC mapr-commons LLVM ${LLVM_LIBS} clang-cpp mapr-util)
target_link_libraries(mapr-util PUBLIC mapr-commons)


separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
target_compile_definitions(mapr-commons INTERFACE ${LLVM_DEFINITIONS_LIST})

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(mapr-commons INTERFACE DEV_MODE)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Debug AND CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()

# ---- Declare executable ----

set(MAPR_SOURCESET
        src/app/cli/cliOptionsParser.cpp
        src/app/cli/cliOptionsParser.hpp
        src/app/cli/commandLineException.cpp
        src/app/cli/commandLineException.hpp
        src/app/cli/utils.cpp
        src/app/cli/utils.hpp
        src/app/main.cpp
        src/app/parser/sourceParser.cpp
        src/app/parser/sourceParser.hpp
        src/app/pipeline/declarationsPipeline.cpp
        src/app/pipeline/declarationsPipeline.hpp
        src/app/pipeline/definitionsPipeline.cpp
        src/app/pipeline/definitionsPipeline.hpp
        src/app/pipeline/pipelineBase.cpp
        src/app/pipeline/pipelineBase.hpp
        src/app/sanitizer/codeSanitizer.cpp
        src/app/sanitizer/codeSanitizer.hpp
        )

add_executable(mapr ${MAPR_SOURCESET})
add_executable(mapr::mapr ALIAS mapr)

set_target_properties(
        mapr PROPERTIES
        OUTPUT_NAME mapr
        EXPORT_NAME mapr
)

target_compile_features(mapr PRIVATE cxx_std_20)

target_link_libraries(mapr PRIVATE
        mapr-config
        mapr-match
        mapr-transform
        mapr-util
        mapr-view
        )

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(mapr PRIVATE RESOURCES_DIR="$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/resources>")
endif ()

# ---- Install rules ----

if (NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/install-rules.cmake)
endif ()

# ---- Developer mode ----

if (NOT mapr_DEVELOPER_MODE)
    return()
elseif (NOT PROJECT_IS_TOP_LEVEL)
    message(
            AUTHOR_WARNING
            "Developer mode is intended for developers of mapr"
    )
endif ()

include(cmake/dev-mode.cmake)
