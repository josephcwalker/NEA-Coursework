workspace "Connect 4 With AI"
	configurations { "Debug", "Release" }
	architecture "x64"

os.execute [[vendor\CMake\bin\cmake -DBUILD_SHARED_LIBS=OFF -S "Connect 4\vendor\SFML" -B "Connect 4\vendor\SFML" -G "MinGW Makefiles"]]

externalproject "SFML"
	location "Connect 4/vendor/SFML"
	kind "StaticLib"
	language "C++"

project "Connect 4"
	kind "ConsoleApp"
	language "C++"

	location "%{prj.name}"
	targetname "Connect4"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin/%{cfg.buildcfg}-obj"

	systemversion "latest"
	cppdialect "C++17"

	debugdir "%{prj.name}/"

	files
	{
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/SFML/include",
		"%{prj.name}/vendor/Eigen",
		"%{prj.name}/vendor"
	}

	libdirs
	{
		"%{prj.name}/vendor/SFML/lib",
		"%{prj.name}/vendor/Freetype/release static/win64"
	}

	defines "SFML_STATIC"

	links
	{
		"stdc++fs",
		"sfml-graphics-s",
		"sfml-window-s",
		"sfml-system-s",
		"opengl32",
		"freetype",
		"winmm",
		"gdi32"
	}

	filter "configurations:Debug"
		symbols "On"
		defines "DEBUG"

	filter "configurations:Release"
		optimize "On"
		defines "NDEBUG"