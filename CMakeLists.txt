cmake_minimum_required(VERSION 3.27)

project(mymuduo
  LANGUAGES CXX
  DESCRIPTION "仿照muduo库而构建的一个网络库 ")

set(CXX_STANDARD_REQUIRED 23)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
aux_source_directory(. SRC_LIST)
add_library(mymuduo SHARED ${SRC_LIST})