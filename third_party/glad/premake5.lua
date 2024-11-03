project "glad"

language "c"

kind "staticlib"

targetdir "%{wks.location}/third_party/build/"
objdir "%{wks.location}/third_party/build/obj/%{prj.name}-%{cfg.buildcfg}"

files {
    "src/**.c"
}

includedirs {
    "include/"
}

filter "configurations:dev"
defines { "_DEBUG" }
symbols "on"
optimize "off"

filter "configurations:rel"
defines { "_NDEBUG" }
symbols "off"
optimize "full"
