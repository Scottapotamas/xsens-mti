cmake_minimum_required(VERSION 3.6)

SET( PROJECT_VERSION 0.0.8 )

project(xsens-mti)

add_library(xsens-mti STATIC src/xsens_mti.c src/xsens_utility.c src/xsens_mdata2.c)

target_include_directories(xsens-mti PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
