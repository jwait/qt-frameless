find_package(Qt5 COMPONENTS Core Widgets Gui)

set(PROJECT_NAME qframeless)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(header_path 
      ${CMAKE_CURRENT_SOURCE_DIR}/include/qframeless
)

set(headers 
    ${header_path}/gframeless_global.h
    ${header_path}/qframelessdialog.h
    ${header_path}/qframelesshelper.h
    ${header_path}/qframelessmainwindow.h
    ${header_path}/qframelesswidget.h
    ${header_path}/qshadowhelper.h
)

set(sources
      ./src/qframelessdialog.cpp
      ./src/qframelesshelper.cpp
      ./src/qframelessmainwindow.cpp
      ./src/qframelesswidget.cpp
      ./src/qshadowhelper.cpp
)

add_library(${PROJECT_NAME} SHARED ${headers})
target_compile_definitions(${PROJECT_NAME} 
  PRIVATE 
    QFRAMELESS_EXPORT
)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${sources}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
  PRIVATE
)

# #####
# Moc
# #
qt5_wrap_cpp(frameless_moc
    ${headers}
    TARGET ${PROJECT_NAME}
    OPTIONS --no-notes # Don't display a note for the headers which don't produce a moc_*.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${frameless_moc})

###############
# Installation
##

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/$<CONFIG>
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/$<CONFIG>
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/$<CONFIG>
        )

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
)

