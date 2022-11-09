set_project("Picidae")
set_languages("c++17")
set_warnings("allextra")
--set_warnings("all", "error")

set_config("cc", "clang")
set_config("cxx", "clang++")
set_config("ld", "clang++")

add_rules("mode.debug", "mode.release")

add_includedirs("./")

add_requires("gtest")

add_subdirs('test')
