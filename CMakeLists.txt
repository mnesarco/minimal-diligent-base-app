cmake_minimum_required (VERSION 3.19)

project(myapp)

set(SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

set(INCLUDE
    ${CMAKE_CURRENT_LIST_DIR}/src/main.hpp
)

option(DILIGENT_BUILD_TOOLS         "Build DiligentTools module"    ON)
option(DILIGENT_BUILD_FX            "Build DiligentFX module"       ON)
option(DILIGENT_BUILD_SAMPLES       "Build DiligentSamples module"  OFF)
option(DILIGENT_BUILD_DEMOS         "Build demo projects"           OFF)
option(DILIGENT_BUILD_UNITY_PLUGIN  "Build Unity plugin"            OFF)

add_subdirectory(3rdparty/DiligentEngine)
add_subdirectory(3rdparty/DiligentBaseApp)

add_target_platform_app(${PROJECT_NAME} "${SOURCE}" "${INCLUDE}" "")

if(PLATFORM_WIN32)
    copy_required_dlls(${PROJECT_NAME})
    append_base_win32_source(${PROJECT_NAME})
elseif(PLATFORM_UNIVERSAL_WINDOWS)
    append_base_uwp_source(${PROJECT_NAME})
    package_required_dlls(${PROJECT_NAME})
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE	
        src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        # On Linux we must have Diligent-NativeAppBase go first, otherwise the linker 
        # will fail to resolve Diligent::CreateApplication() function.
        Diligent-NativeAppBase
        Diligent-BuildSettings
        Diligent-BaseApp
)
set_common_target_properties(${PROJECT_NAME})

if(MSVC)
    # Disable MSVC-specific warnings
    # - w4201: nonstandard extension used: nameless struct/union
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4201)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    FOLDER ${CMAKE_CURRENT_SOURCE_DIR}
)

source_group("src" FILES ${SOURCE} ${INCLUDE})

if(PLATFORM_MACOS AND VULKAN_LIB_PATH)
    # Configure rpath so that executables can find vulkan library
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_RPATH "${VULKAN_LIB_PATH}"
    )
endif()

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        DiligentFX
)

