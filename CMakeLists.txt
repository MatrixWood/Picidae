cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")

project(
  picidae
  VERSION 0.1.0
  LANGUAGES CXX
)

set(project_version "${${PROJECT_NAME}_VERSION}")


message(STATUS "Project '${PROJECT_NAME}', version: '${project_version}'")

option(PICIDAE_OPT_BUILD_TESTS "Build all picidae tests" ON)

message(STATUS "PICIDAE_OPT_BUILD_TESTS is ${PICIDAE_OPT_BUILD_TESTS}")

# CMake helpers:
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(${PROJECT_NAME} INTERFACE)

message(STATUS "PROJECT_SOURCE_DIR is '${PROJECT_SOURCE_DIR}'")

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE
  ${PROJECT_SOURCE_DIR}
)

target_sources(
  ${PROJECT_NAME}
  INTERFACE
)

if (PICIDAE_OPT_BUILD_TESTS)
  add_subdirectory(test)
endif()