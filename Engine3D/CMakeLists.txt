set(PROJECT_NAME Engine3D)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "BillboardObject.hpp"
    "Cache.hpp"
    "Camera.hpp"
    "Canvas.hpp"
    "Collision.hpp"
    "Cubemap.hpp"
    "dirent.h"
    "Engine3D.hpp"
    "FBObject.hpp"
    "File.hpp"
    "FPSLimiter.hpp"
    "Game.hpp"
    "Gamepad.hpp"
    "JSONDocument.hpp"
    "Macros.hpp"
    "Mesh.hpp"
    "Music.hpp"
    "Plane.hpp"
    "Save.hpp"
    "SceneObject.hpp"
    "Shader.hpp"
    "Shapes.hpp"
    "Sound.hpp"
    "SpatialPartition.hpp"
    "Sprite.hpp"
    "System.hpp"
    "Text.hpp"
    "Texture.hpp"
    "Types.hpp"
    "Utility.hpp"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "BillboardObject.cpp"
    "Camera.cpp"
    "Canvas.cpp"
    "Collision.cpp"
    "Cubemap.cpp"
    "FBObject.cpp"
    "File.cpp"
    "FPSLimiter.cpp"
    "Game.cpp"
    "Gamepad.cpp"
    "JSONDocument.cpp"
    "Mesh.cpp"
    "Music.cpp"
    "Save.cpp"
    "SceneObject.cpp"
    "Shader.cpp"
    "Sound.cpp"
    "Sprite.cpp"
    "System.cpp"
    "Text.cpp"
    "Texture.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Engine3D)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CRT_SECURE_NO_WARNINGS;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi
            >
            /permissive-;
            /std:c++latest;
            /sdl;
            /arch:AVX2;
            /Gy;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

