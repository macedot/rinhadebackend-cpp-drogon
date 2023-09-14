cmake_minimum_required(VERSION 3.15)
project(rinhadebackend-cpp-drogon)

find_package(Drogon REQUIRED)
find_package(fmt REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(ZLIB REQUIRED)

add_executable(${PROJECT_NAME} src/main.cc)
target_link_libraries(${PROJECT_NAME} Drogon::Drogon)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
target_link_libraries(${PROJECT_NAME} PostgreSQL::PostgreSQL)
target_link_libraries(${PROJECT_NAME} ZLIB::ZLIB)
