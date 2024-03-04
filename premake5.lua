workspace "GameEngine"
    architecture "x64"
    startproject "EngineEditor"

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
IncludeDir['glm'] = "GameEngine/vendor/glm"
IncludeDir['stb_image'] = "GameEngine/vendor/stb_image"
IncludeDir['entt'] = "GameEngine/vendor/entt/include"
IncludeDir['box2d'] = "GameEngine/vendor/box2d/include"

include "GameEngine/vendor/GLFW"
include "GameEngine/vendor/Glad"
include "GameEngine/vendor/imgui"
include "GameEngine/vendor/box2d"

project "GameEngine"
    location "GameEngine"    
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" ..outputdir.. "/%{prj.name")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    pchheader "Enginepch.h"
    pchsource "GameEngine/src/Enginepch.cpp"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.box2d}"
        }

    links{
        "GLFW",
        "Glad",
        "imGui",
        "opengl32.lib",
        "box2d"
    }

    filter "system:windows"
        systemversion "latest"
        defines{
            "ENGINE_PLATFORM_WINDOWS",
            "ENGINE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }


    filter "configurations:Debug"
        defines "ENGIEN_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ENGIEN_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        runtime "Release"
        optimize "on"
    

project "EngineEditor"
    location "EngineEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" ..outputdir.. "/%{prj.name")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs{
        "GameEngine/vendor/spdlog/include",
        "GameEngine/src",
        "GameEngine/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}"
    }

    links{
        "GameEngine"
    }

    filter "system:windows"
        systemversion "latest"
        defines{
            "ENGINE_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "ENGIEN_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ENGIEN_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        runtime "Release"
        optimize "on"

