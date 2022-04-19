workspace "whttp"
  location "project/"
  configurations {"debug", "release"}
  language "C"
  cdialect "C11"
  flags { "MultiProcessorCompile", "FatalWarnings" }

filter { "system:windows" }
  platforms { "win64" }

filter { "platforms:win64" }
defines { "WIN32", "_CRT_SECURE_NO_WARNINGS", "WIN32_LEAN_AND_MEAN", "NOMINMAX" }
architecture "x64"

filter "configurations:debug"
  defines { "WHTTP_DEBUG" }
  symbols "On"

filter "configurations:release"
  optimize "On"

project "whttp"
  location "project/"
  kind "ConsoleApp"
  targetdir "project/bin/%{cfg.platform}/%{cfg.buildcfg}"
  files {"src/**.h", "src/**.c"}
  includedirs { "src" }
  editandcontinue "Off"
  staticruntime "On"
  flags { "NoIncrementalLink", "NoPCH" }
