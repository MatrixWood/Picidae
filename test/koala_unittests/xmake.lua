add_packages("gtest")

target("typeinfo_test")
    add_files("type_info_test.cc")
    add_deps("koala")

target("define_test")
    add_files("define_test.cc")
    add_deps("koala")

target("boxed_value_test")
    add_files("boxed_value_test.cc")
    add_deps("koala")

target("boxed_number_test")
    add_files("boxed_number_test.cc")
    add_deps("koala")

target("function_params_test")
    add_files("function_params_test.cc")
    add_deps("koala")