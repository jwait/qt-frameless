set(PROJECT_NAME demo)

find_package(Qt5 COMPONENTS Core Widgets Gui)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

list(APPEND source_dirs
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/skin
)

foreach(source_dir IN ITEMS ${source_dirs})
  list(APPEND source_header_path "${source_dir}/*.h" "${source_dir}/*.hpp")
  list(APPEND source_source_path "${source_dir}/*.c" "${source_dir}/*.cpp" "${source_dir}/*.ui")
  list(APPEND source_resource_path "${source_dir}/*.qrc" "${source_dir}/*.qss")
  list(APPEND source_ts_path "${source_dir}/*.ts")
endforeach()

file(GLOB_RECURSE headers ${source_header_path})
file(GLOB_RECURSE sources  ${source_source_path})
file(GLOB_RECURSE resources ${source_resource_path})
file(GLOB_RECURSE ts_files ${source_ts_path})

list(APPEND all_file_path ${headers} ${sources} ${resources} ${ts_files})

# Turn on the switch that allows you to create folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# create folder tree
foreach(_source IN ITEMS ${all_file_path})
  if (IS_ABSOLUTE "${_source}")
    file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
  else()
    set(_source_rel "${_source}")
  endif()
  get_filename_component(_source_path "${_source_rel}" PATH)
  string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
  source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

add_executable(${PROJECT_NAME} WIN32 "")
target_compile_definitions(${PROJECT_NAME} PRIVATE QFRAMELESS_IMPORT)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${all_file_path}
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    qframeless
)

# copy qt dependency
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND "$<TARGET_FILE_DIR:Qt5::qmake>/windeployqt.exe" "--no-translations" $<TARGET_FILE:${PROJECT_NAME}>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# copy other dependency
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)
