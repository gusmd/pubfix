cmake_minimum_required(VERSION 3.2)
project(pubfix_main)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(QHotkey)
add_subdirectory(pubfix)

# Grab Qt and MSVC dlls
include(GetDependencies)
GetDependencies(TARGET pubfix)

# Run install target post-build
add_custom_target(deploy DEPENDS pubfix getdependencies)
add_custom_command(TARGET deploy POST_BUILD
    COMMAND ${CMAKE_COMMAND} --build . --config Release --target install
)
# don't build these targets everytime on VS
set_target_properties(deploy getdependencies
    PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE
)
# Set pubfix as the startup project on VS solution
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT pubfix)
