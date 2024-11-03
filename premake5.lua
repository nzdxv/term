workspace "term"

architecture "x86_64"

configurations { "dev", "rel" }

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
    "src/"
}

links {

}

filter "configurations:dev"
defines { "_DEV" }
symbols "on"
optimize "off"

filter "configurations:rel"
defines { "_REL" }
symbols "off"
optimize "full"
