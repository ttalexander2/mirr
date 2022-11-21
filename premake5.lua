workspace "mirr"
    architecture "x64"
    startproject "example"
    configurations {
            "Debug",
            "Release"
     }
    flags {
    	"MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "mirr"
    location "mirr"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.hpp",
        "src/**.inl",
        "src/**.cpp",
    }

    includedirs
    {
        "include"
    }

    filter "configurations:Debug"
           runtime "Debug"
           symbols "on"

    filter "configurations:Release"
           runtime "Release"
           optimize "on"

project "example"
    location "example"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    links
    {
        "mirr"
    }

    files
    {
        "example/example.cpp"
    }

    includedirs
    {
        "include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
