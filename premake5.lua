workspace "Connect 4 With AI"
	configurations { "Debug", "Release" }
	architecture "x64"

project "Connect 4"
	kind "ConsoleApp"
	language "C++"

	location "%{prj.name}"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin/%{cfg.buildcfg}-obj"

	files
	{
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src"
	}

	filter "configurations:Debug"
		symbols "On"
		defines "DEBUG"

	filter "configurations:Release"
		optimize "On"
		defines "NDEBUG"