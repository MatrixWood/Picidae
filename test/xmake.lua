add_packages("gtest")

target("common_macro_test")
  add_files("common_macro_test.cc")

target("cpplisp_test")
  add_files("cpplisp_test.cc")

target("meta_test")
  add_files("meta_test.cc")

target("lightweighttest_test")
  add_files("lightweighttest_test.cc")


target("json_unittest")
  add_files("json_unittest/json_unittest.cc")