function(GetDependencies)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs TARGET)
    cmake_parse_arguments(GETDEPENDENCIES
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    if(NOT GETDEPENDENCIES_TARGET)
        message(FATAL_ERROR "A TARGET must be specified")
    endif()
    
    # Find windeployqt
    find_program(GETDEPENDENCIES_PROGRAM windeployqt HINTS ${QTDIR})
    if(GETDEPENDENCIES_PROGRAM)
        message(STATUS "Found ${GETDEPENDENCIES_PROGRAM}")
    else()
        message(FATAL_ERROR "Unable to find windeployqt")
    endif()

    # Windeploytqt arguments
    set(GETDEPENDENCIES_OUTPUT_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/dependencies/")
    list(APPEND GETDEPENDENCIES_ARGS 
        --no-compiler-runtime
        --no-translations 
        --no-system-d3d-compiler 
        --no-opengl-sw 
        --no-svg 
        --dir 
        ${GETDEPENDENCIES_OUTPUT_FOLDER}
    )

    # Gather file names of the executables of each target
    foreach(target_project ${GETDEPENDENCIES_TARGET})
        list(APPEND TARGET_EXES $<TARGET_FILE:${target_project}>)
    endforeach()

    
    add_custom_target(getdependencies
        ${GETDEPENDENCIES_PROGRAM} ${GETDEPENDENCIES_ARGS} ${TARGET_EXES}
        DEPENDS ${GETDEPENDENCIES_TARGET}
    )

    # MSVC Runtime
    get_filename_component(VS_RUNTIME_FOLDER ${CMAKE_CXX_COMPILER} DIRECTORY)

    # Assuming VS2015+
    add_custom_command(TARGET getdependencies
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${VS_RUNTIME_FOLDER}/vcruntime140.dll ${GETDEPENDENCIES_OUTPUT_FOLDER}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${VS_RUNTIME_FOLDER}/msvcp140.dll ${GETDEPENDENCIES_OUTPUT_FOLDER}/
    )

    install(DIRECTORY "${GETDEPENDENCIES_OUTPUT_FOLDER}/"
        DESTINATION .
        COMPONENT ${GETDEPENDENCIES_TARGET}
    )
endfunction()
