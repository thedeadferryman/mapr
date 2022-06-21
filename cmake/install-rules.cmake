if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR include/mapr CACHE PATH "")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package mapr)

install(
    TARGETS
        mapr
        mapr-commons
        mapr-config
        mapr-match
        mapr-transform
        mapr-util
        mapr-view
    EXPORT maprTargets
    RUNTIME COMPONENT mapr_Runtime
)

install(DIRECTORY resources/ DESTINATION share/mapr/resources)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    mapr_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(mapr_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${mapr_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT mapr_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${mapr_INSTALL_CMAKEDIR}"
    COMPONENT mapr_Development
)

install(
    EXPORT maprTargets
    NAMESPACE mapr::
    DESTINATION "${mapr_INSTALL_CMAKEDIR}"
    COMPONENT mapr_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(cmake/project-info.cmake)
  include(CPack)
endif()
