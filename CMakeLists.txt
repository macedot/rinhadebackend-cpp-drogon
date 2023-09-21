cmake_minimum_required(VERSION 3.15)
project(rinhadebackend-cpp-drogon)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Drogon REQUIRED)
find_package(fmt REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(ZLIB REQUIRED)

add_executable(${PROJECT_NAME} src/main.cc)
target_link_libraries(${PROJECT_NAME} Drogon::Drogon)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
target_link_libraries(${PROJECT_NAME} PostgreSQL::PostgreSQL)
target_link_libraries(${PROJECT_NAME} ZLIB::ZLIB)
