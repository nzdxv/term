--
-- Include directories
includes = {}
includes["glad"] = "%{wks.location}/third_party/glad/include/"

--
-- Workspace
workspace "term"

architecture "x86_64"

configurations { "dev", "rel" }

--
-- Third party
include "third_party/glad"

--
-- Terminal project
project "term"

kind "consoleapp"

language "c"

targetdir "%{wks.location}/build/"
objdir "%{wks.location}/build/obj/%{prj.name}-%{cfg.buildcfg}"

files {
    "src/**.h",
    "src/**.c"
}

includedirs {
    "src/",

    "%{includes.glad}"
}

links {
    "glad",

    "opengl32.lib"
}

filter "configurations:dev"
defines { "_DEV" }
symbols "on"
optimize "off"

filter "configurations:rel"
defines { "_REL" }
symbols "off"
optimize "full"
