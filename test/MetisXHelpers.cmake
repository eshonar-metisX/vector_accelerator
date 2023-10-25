include(FetchContent)

function(metisx_fetch_external name)
  FetchContent_Declare(
    ${name}
    ${ARGN}
  )
  FetchContent_MakeAvailable(${name})
endfunction()

function(get_link_libraries OUTPUT_LIST TARGET)
    get_target_property(IMPORTED ${TARGET} IMPORTED)
    list(APPEND VISITED_TARGETS ${TARGET})
    if (IMPORTED)
        get_target_property(LIBS ${TARGET} INTERFACE_LINK_LIBRARIES)
    else()
        get_target_property(LIBS ${TARGET} LINK_LIBRARIES)
    endif()
    set(${OUTPUT_LIST} ${LIBS} PARENT_SCOPE)
endfunction()

function(metisx_add_shared_lib project_name)
    foreach(LIB IN LISTS ARGN)
        if (NOT LIB MATCHES "metisx_.*|mu" )
            continue()
        endif()
        __metisx_add_shared_lib(${project_name} ${LIB})
    endforeach()
endfunction()

function(__metisx_add_shared_lib project_name lib_name)
    target_link_options(${project_name} PRIVATE
    "$<$<CXX_COMPILER_ID:GNU>:SHELL:LINKER:--whole-archive $<TARGET_LINKER_FILE:${lib_name}> LINKER:--no-whole-archive>"
    )
    target_link_libraries(${project_name} PRIVATE ${lib_name})
endfunction()



function(metisx_add_test name)
    # if (INSTALL_MODE STREQUAL "0")
        set(options)
        set(oneValueArgs)
        set(multiValueArgs INCLUDE LIBRARY SOURCE DEFINITIONS)
        cmake_parse_arguments(METISX_TEST "${options}" "${oneValueArgs}"
                            "${multiValueArgs}" ${ARGN} )

        set(name ${name}_test)

        add_executable(${name} ${METISX_TEST_SOURCE} ${METISX_SDK_PATH}/test/test_main.cpp)
        message(STATUS "${name} ${METISX_TEST_DEFINITIONS}")
        target_compile_definitions(${name} PRIVATE _SIM_=1 ${METISX_TEST_DEFINITIONS})
        target_link_libraries(${name} PRIVATE gtest fmt::fmt metisx dl ${METISX_TEST_LIBRARY})
        target_include_directories(${name} PRIVATE ${METISX_TEST_INCLUDE})
        target_include_directories(${name} PRIVATE ${METISX_SDK_PATH}/metisx)
    # endif()
endfunction()

# add_mu_library(<name>
#             [LIB|EXE]
#             [LINKER <linker>]
#             [STARTUP <startup>]
#             [INCLUDE <include>...]
#             [LIBRARY <library>...]
#             [SOURCE <source>...])
function(add_mu_binary name)
    set(options LIB EXE)
    set(oneValueArgs LINKER STARTUP)
    set(multiValueArgs INCLUDE LIBRARY SOURCE DEFINITIONS)
    cmake_parse_arguments(METISX_MU "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN} )

    # message("METISX_MU_INCLUDE=${METISX_MU_INCLUDE}")
    # message("METISX_MU_LIBRARY=${METISX_MU_LIBRARY}")
    # message("METISX_MU_SOURCE=${METISX_MU_SOURCE}")
    # message("METISX_MU_LINKER=${METISX_MU_LINKER}")

    set(COMMAND python3 ${METISX_SDK_PATH}/mu/build.py --output ${name} --path ${CMAKE_CURRENT_LIST_DIR})

    if (${METISX_MU_LIB})
        set(COMMAND ${COMMAND} --lib)
    endif()

    if (DEFINED METISX_MU_LINKER)
        set(COMMAND ${COMMAND} --linker ${METISX_MU_LINKER})
    endif()

    if (DEFINED METISX_MU_STARTUP)
        set(COMMAND ${COMMAND} --startup ${METISX_MU_STARTUP})
    endif()

    if (DEFINED METISX_MU_LIBRARY)
        set(COMMAND ${COMMAND} --library)
        foreach(lib IN LISTS METISX_MU_LIBRARY)
            set(DEPENDENCIES ${DEPENDENCIES} ${lib}_mu)
            set(COMMAND ${COMMAND} ${lib})
        endforeach()
    endif()

    if (DEFINED METISX_MU_INCLUDE)
        set(COMMAND ${COMMAND} --include)
        foreach(inc IN LISTS METISX_MU_INCLUDE)
            set(COMMAND ${COMMAND} ${inc})
        endforeach()
    endif()

    if (DEFINED METISX_MU_SOURCE)
        set(COMMAND ${COMMAND} --source)
        foreach(src IN LISTS METISX_MU_SOURCE)
            set(COMMAND ${COMMAND} ${src})
        endforeach()
    endif()

    if (DEFINED METISX_MU_DEFINITIONS)
        set(COMMAND ${COMMAND} --definitions)
        foreach(def IN LISTS METISX_MU_DEFINITIONS)
            set(COMMAND ${COMMAND} ${def})
        endforeach()
    endif()


    if ((${METISX_MU_LIB}) OR (NOT ${name} MATCHES "master.*|admin.*"))
        add_custom_command(
            OUTPUT BUILD_${name}
            COMMAND ${COMMAND}
            DEPENDS ${DEPENDENCIES} mu_lib
        )
    else()
        message(STATUS "BUILD_${name} ${METISX_SDK_PATH}/mu/out/binary/${name}_binary.h to ${METISX_SDK_PATH}/metisx/firmware/bsp/mu_binary/${name}_binary.h")
        add_custom_command(
            OUTPUT BUILD_${name}
            COMMAND ${COMMAND}
            COMMAND ${CMAKE_COMMAND} -E copy ${METISX_SDK_PATH}/mu/out/binary/${name}_binary.h
                        ${METISX_SDK_PATH}/metisx/firmware/bsp/mu_binary/${name}_binary.h
            DEPENDS ${DEPENDENCIES} mu_lib
        )
    endif()

    add_custom_target(${name}_mu ALL
        DEPENDS BUILD_${name}
        DEPENDS mu_lib
    )

    add_library(${name} STATIC ${METISX_MU_SOURCE})
    if (${name} STRLESS "mu")
        add_dependencies(${name} ${name}_mu)
    endif()
    target_link_libraries(${name} PRIVATE ${METISX_MU_LIBRARY})
    target_link_libraries(${name} PUBLIC fmt::fmt)
    target_compile_definitions(${name} PRIVATE _SIM_=1 NDEBUG ${METISX_MU_DEFINITIONS})
    target_include_directories(${name} PRIVATE ${METISX_MU_INCLUDE})
endfunction()

# add_mu_library(<name>
#                     [INCLUDE <include>...]
#                     [LIBRARY <library>...]
#                     [SOURCE <source>...])
function(add_mu_library name)
    add_mu_binary(${name} LIB ${ARGN})
endfunction()

# add_ms_executable(<name>
#                        [INCLUDE <include>...]
#                        [LIBRARY <library>...]
#                        [SOURCE <source>...])
function(add_mu_executable name)
    add_mu_binary(${name} EXE ${ARGN})
endfunction()