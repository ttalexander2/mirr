project "mirr"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("obj/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.inl",
        "src/**.cpp",
        "include/**.h",
        "include/**.hpp",
        "include/**.inl",
        "include/**.cpp",
    }

    includedirs 
    {
        "include",
        "src"
    }

    filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

    filter "configurations:Release"
    runtime "Release"
    optimize "on"
