cmake_minimum_required(VERSION 3.7)
project(mse2cdb)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 17)

find_package(sqlite3 REQUIRED)
find_package(Lua REQUIRED)
find_package(LibZip REQUIRED)

include_directories(./)

set(MAIN_FILES main.cpp)
set(IO_FILES io/CDBAccess.cpp io/CDBAccess.hpp io/MSEReader.cpp io/MSEReader.hpp)
set(DOMAIN_FILES domain/MSEDataNode.cpp domain/MSEDataNode.hpp domain/CDBCard.hpp)
set(SERVICE_FILES service/LuaCardData.cpp service/LuaCardData.hpp service/LuaMSENodeData.cpp service/LuaMSENodeData.hpp service/LuaConverters.hpp service/LuaConverters.cpp)

add_executable(mse2cdb ${MAIN_FILES} ${IO_FILES} ${DOMAIN_FILES} ${SERVICE_FILES})
target_link_libraries(mse2cdb ${SQLITE3_LIBRARIES} ${LUA_LIBRARIES} ${LIBZIP_LIBRARIES})