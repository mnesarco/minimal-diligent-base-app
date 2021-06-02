cmake_minimum_required (VERSION 3.6)

project(Diligent-BaseApp)

if(PLATFORM_WIN32)
    set(SOURCE 
        src/Win32/ApplicationBackendWin32.cpp
        src/Win32/InputControllerWin32.cpp
        src/ApplicationBackend.cpp
    )
    set(INCLUDE 
        include/ApplicationBackend.hpp
        include/Win32/InputControllerWin32.hpp
    )
    set(WIN32_RESOURCES
        src/Win32/resources/Win32AppResource.h
        src/Win32/resources/directx11-logo.bmp
        src/Win32/resources/directx12-logo.bmp
        src/Win32/resources/vulkan-logo.bmp
        src/Win32/resources/opengl-logo.bmp
        src/Win32/resources/Win32AppResource.rc
    )

    function(append_base_win32_source TARGET_NAME)
        get_target_property(SAMPLE_BASE_SOURCE_DIR ${PROJECT_NAME} SOURCE_DIR)
        # We need to add .res file to executable target to make resources available
        set(RES_FILE ${SAMPLE_BASE_SOURCE_DIR}/src/Win32/resources/Win32AppResource.rc)
        target_sources(${TARGET_NAME} PRIVATE ${RES_FILE})
        source_group("resources" FILES ${RES_FILE})
    endfunction()

elseif(PLATFORM_UNIVERSAL_WINDOWS)

    # Windows Runtime types cannot be included into static libraries
    # https://social.msdn.microsoft.com/Forums/en-US/269db513-64ef-4817-a025-43954f614eb3/lnk4264-why-are-static-libraries-not-recommended-when-authoring-windows-runtime-types?forum=winappswithnativecode
    # So as a workaround, we will include all source files into the target app project
    function(append_base_uwp_source TARGET_NAME)
        get_target_property(SAMPLE_BASE_SOURCE_DIR ${PROJECT_NAME} SOURCE_DIR)

        set(SAMPLE_BASE_UWP_SOURCE
            ${SAMPLE_BASE_SOURCE_DIR}/src/UWP/ImguiUWPEventHelper.cpp
            ${SAMPLE_BASE_SOURCE_DIR}/src/UWP/ApplicationBackendUWP.cpp
            ${SAMPLE_BASE_SOURCE_DIR}/src/UWP/InputControllerEventHandlerUWP.cpp
            ${SAMPLE_BASE_SOURCE_DIR}/src/ApplicationBackend.cpp
        )

        set(SAMPLE_BASE_UWP_INCLUDE
            ${SAMPLE_BASE_SOURCE_DIR}/src/UWP/ImguiUWPEventHelper.h
            ${SAMPLE_BASE_SOURCE_DIR}/src/UWP/InputControllerEventHandlerUWP.h
            ${SAMPLE_BASE_SOURCE_DIR}/include/ApplicationBackend.hpp
            ${SAMPLE_BASE_SOURCE_DIR}/include/UWP/InputControllerUWP.hpp
        )

        set(SAMPLE_BASE_UWP_INCLUDE_DIR
            ${SAMPLE_BASE_SOURCE_DIR}/src/UWP
        )

        target_sources(${TARGET_NAME} PRIVATE ${SAMPLE_BASE_UWP_SOURCE} ${SAMPLE_BASE_UWP_INCLUDE})
        source_group("src\\Application" FILES ${SAMPLE_BASE_UWP_SOURCE})
        source_group("include\\Application" FILES ${SAMPLE_BASE_UWP_INCLUDE})
        target_include_directories(${TARGET_NAME} PRIVATE ${SAMPLE_BASE_UWP_INCLUDE_DIR})
    endfunction()

elseif(PLATFORM_ANDROID)
    set(SOURCE
        src/Android/InputControllerAndroid.cpp
        src/Android/ApplicationBackendAndroid.cpp
        src/ApplicationBackend.cpp
    )
    set(INCLUDE 
        include/Android/InputControllerAndroid.hpp
        include/ApplicationBackend.hpp
    )
elseif(PLATFORM_LINUX)
    set(SOURCE 
        src/Linux/InputControllerLinux.cpp
        src/Linux/ApplicationBackendLinux.cpp
        src/ApplicationBackend.cpp
    )
    set(INCLUDE 
        include/Linux/InputControllerLinux.hpp
        include/ApplicationBackend.hpp
    )
elseif(PLATFORM_MACOS)

    set(SOURCE
        src/MacOS/InputControllerMacOS.cpp
        src/MacOS/ApplicationBackendMacOS.mm
        src/ApplicationBackend.cpp
    )
    set(INCLUDE
        Include/MacOS/InputControllerMacOS.hpp
        include/ApplicationBackend.hpp
    )

elseif(PLATFORM_IOS)
    set(SOURCE
        src/IOS/InputControllerIOS.cpp
        src/IOS/ApplicationBackendIOS.cpp
        src/ApplicationBackend.cpp
    )
    set(INCLUDE
        include/IOS/InputControllerIOS.hpp
        include/ApplicationBackend.hpp
    )

endif()

list(APPEND SOURCE
    src/FirstPersonCamera.cpp
    src/Application.cpp
)

list(APPEND INCLUDE
    include/FirstPersonCamera.hpp
    include/InputController.hpp
    include/Application.hpp
)


add_library(${PROJECT_NAME} STATIC ${SOURCE} ${INCLUDE})
set_common_target_properties(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
PUBLIC
    include
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -DUNICODE)
    
    if(PLATFORM_UNIVERSAL_WINDOWS)
        # Disable w4189: local variable is initialized but not referenced
        # Disable w4063: case is not a valid value for switch of enum
        # Consume the windows runtime extensions (/ZW)
        target_compile_options(${PROJECT_NAME} INTERFACE /wd4189 /wd4063 /ZW)
    endif()
endif()

get_supported_backends(ENGINE_LIBRARIES)

target_link_libraries(${PROJECT_NAME} 
PRIVATE 
    Diligent-BuildSettings
PUBLIC
    Diligent-Common
    Diligent-GraphicsTools
    Diligent-TextureLoader
    Diligent-TargetPlatform
    Diligent-Imgui
    ${ENGINE_LIBRARIES}
    Diligent-NativeAppBase
)

if(PLATFORM_UNIVERSAL_WINDOWS)
    target_link_libraries(${PROJECT_NAME} PRIVATE dxguid.lib)
elseif(PLATFORM_ANDROID)
    target_link_libraries(${PROJECT_NAME} PRIVATE GLESv3 PUBLIC native_app_glue)
elseif(PLATFORM_LINUX)
    target_link_libraries(${PROJECT_NAME} PRIVATE XCBKeySyms GL X11)
elseif(PLATFORM_MACOS OR PLATFORM_IOS)

endif()

source_group("src" FILES ${SOURCE})
source_group("include" FILES ${INCLUDE})

if(PLATFORM_WIN32)
    target_sources(${PROJECT_NAME} PRIVATE ${WIN32_RESOURCES})
    source_group("resources" FILES ${WIN32_RESOURCES})
endif()

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     FOLDER DiligentSamples
# )
