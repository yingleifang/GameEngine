workspace "GameEngine"
    architecture "x64"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir['GLFW'] = "GameEngine/vendor/GLFW/include"
IncludeDir['Glad'] = "GameEngine/vendor/Glad/include"
IncludeDir['ImGui'] = "GameEngine/vendor/imgui"

include "GameEngine/vendor/GLFW"
include "GameEngine/vendor/Glad"
include "GameEngine/vendor/imgui"

project "GameEngine"
    location "GameEngine"    
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" ..outputdir.. "/%{prj.name")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    pchheader "Enginepch.h"
    pchsource "GameEngine/src/Enginepch.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
    }

    links{
        "GLFW",
        "Glad",
        "imGui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "On"
        systemversion "latest"
        defines{
            "ENGINE_PLATFORM_WINDOWS",
            "ENGINE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} \" ../bin/" ..outputdir.. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "ENGIEN_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "ENGIEN_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        buildoptions "/MD"
        optimize "On"
    
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" ..outputdir.. "/%{prj.name")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs{
        "GameEngine/vendor/spdlog/include",
        "GameEngine/src"
    }

    links{
        "GameEngine"
    }

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "On"
        systemversion "latest"
        defines{
            "ENGINE_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "ENGIEN_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "ENGIEN_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        runtime "Release"
        optimize "On"

    staticruntime "off"
    runtime "Debug"