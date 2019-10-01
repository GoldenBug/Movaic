find_library(HEALTHBAR_LIBRARY "HealthBar" "/usr/lib" "/usr/local/lib")
find_path(HEALTHBAR_INCLUDE_DIR "HealthBar_extension_api.h" "/usr/include" "/usr/local/include")

if ((NOT HEALTHBARK_LIBRARY) OR (NOT HEALTHBAR_INCLUDE_DIR))
    message("Unable to find HealthBar, cloning...")
    execute_process(COMMAND git submodule update --init -- external/HealthBar
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # add and exclude targets
    add_subdirectory(external/HealthBar ${CMAKE_CURRENT_BINARY_DIR}/HealthBar EXCLUDE_FROM_ALL)
    #set_target_properties(api_test PROPERTIES EXCLUDE_FROM_ALL 1)

    # fixup target properties
    target_include_directories(libHealthBar_static SYSTEM PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/HealthBar/src>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/HealthBar/src>)
    target_compile_definitions(libHealthBar_static PUBLIC HEALTHBAR_STATIC_DEFINE)

    # disable some warnings under MSVC, they're very noisy
    if (MSVC)
        target_compile_options(libHealthBar_static PRIVATE /wd4204 /wd4267 /wd4204 /wd4221 /wd4244 /wd4232)
    endif ()
else ()
    add_library(libHealthBar_static INTERFACE)
    target_include_directories(libHealthBar_static INTERFACE ${HEALTHBAR_INCLUDE_DIR})
    target_link_libraries(libHealthBar_static INTERFACE ${HEALTHBAR_LIBRARY})

    # install fake target
    install(TARGETS libHealthBar_static EXPORT standardese DESTINATION ${lib_dir})
endif ()