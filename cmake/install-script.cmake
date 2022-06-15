file(
    RELATIVE_PATH relative_path
    "/${ameko_INSTALL_CMAKEDIR}"
    "/${CMAKE_INSTALL_BINDIR}/${ameko_NAME}"
)

get_filename_component(prefix "${CMAKE_INSTALL_PREFIX}" ABSOLUTE)
set(config_dir "${prefix}/${ameko_INSTALL_CMAKEDIR}")
set(config_file "${config_dir}/amekoConfig.cmake")

message(STATUS "Installing: ${config_file}")
file(WRITE "${config_file}" "\
get_filename_component(
    _ameko_executable
    \"\${CMAKE_CURRENT_LIST_DIR}/${relative_path}\"
    ABSOLUTE
)
set(
    AMEKO_EXECUTABLE \"\${_ameko_executable}\"
    CACHE FILEPATH \"Path to the ameko executable\"
)
")
list(APPEND CMAKE_INSTALL_MANIFEST_FILES "${config_file}")
