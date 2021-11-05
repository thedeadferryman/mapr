if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR include/kodgen CACHE PATH "")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package kodgen)

install(
    TARGETS kodgen_kodgen
    EXPORT kodgenTargets
    RUNTIME COMPONENT kodgen_Runtime
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    kodgen_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(kodgen_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${kodgen_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT kodgen_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${kodgen_INSTALL_CMAKEDIR}"
    COMPONENT kodgen_Development
)

install(
    EXPORT kodgenTargets
    NAMESPACE kodgen::
    DESTINATION "${kodgen_INSTALL_CMAKEDIR}"
    COMPONENT kodgen_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
