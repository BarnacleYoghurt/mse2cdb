cmake_minimum_required(VERSION 3.7)
project(mse2cdb)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 17)

find_package(sqlite3 REQUIRED)
find_package(Lua REQUIRED)

include_directories(./)

set(MAIN_FILES main.cpp)
set(IO_FILES io/CDBAccess.cpp io/CDBAccess.hpp)
set(DOMAIN_FILES domain/MSEDataNode.cpp domain/MSEDataNode.hpp)

add_executable(mse2cdb ${MAIN_FILES} ${IO_FILES} ${DOMAIN_FILES})
target_link_libraries(mse2cdb ${SQLITE3_LIBRARIES} ${LUA_LIBRARIES})