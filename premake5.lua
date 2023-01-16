project "mirr"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("obj/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.inl",
        "src/**.cpp",
    }

    includedirs {
        "include"
    }

    filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

    filter "configurations:Release"
    runtime "Release"
    optimize "on"
