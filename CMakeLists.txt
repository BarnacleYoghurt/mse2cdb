cmake_minimum_required(VERSION 3.7)
project(mse2cdb)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

#set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
#set(WIN_SQLITE3_LIBRARIES /usr/lib/gcc/x86_64-w64-mingw32/5.3-win32/include/c++/x86_64-w64-mingw32/sqlite3.dll)
#set(WIN_LUA53_LIBRARIES /usr/lib/gcc/x86_64-w64-mingw32/5.3-win32/include/c++/x86_64-w64-mingw32/lua5.3/liblua53.a)
#set(WIN_LIBZIP_LIBRARIES /usr/lib/gcc/x86_64-w64-mingw32/5.3-win32/include/c++/x86_64-w64-mingw32/libzip.dll)
#set(WIN_STD_LIBRARIES /usr/lib/gcc/x86_64-w64-mingw32/5.3-win32/libstdc++.a)

find_package(sqlite3 REQUIRED)
find_package(Lua53 REQUIRED)
find_package(LibZip REQUIRED)
find_library(PTHREAD_LIBRARIES NAMES libpthread.a pthread)
find_library(DL_LIBRARIES NAMES libdl.a dl)
find_library(Z_LIBRARIES NAMES libz.a z)

include_directories(./)

set(MAIN_FILES main.cpp)
set(IO_FILES io/CDBAccess.cpp io/CDBAccess.hpp io/MSEReader.cpp io/MSEReader.hpp io/LuaScriptFinder.cpp io/LuaScriptFinder.hpp)
set(DOMAIN_FILES domain/MSEDataNode.cpp domain/MSEDataNode.hpp domain/CDBCard.hpp)
set(SERVICE_FILES service/LuaCardData.cpp service/LuaCardData.hpp service/LuaMSENodeData.cpp service/LuaMSENodeData.hpp service/LuaConverters.hpp service/LuaConverters.cpp)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lua
        DEPENDS ${CMAKE_SOURCE_DIR}/lua
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lua ${CMAKE_CURRENT_BINARY_DIR}/lua)

add_executable(mse2cdb ${MAIN_FILES} ${IO_FILES} ${DOMAIN_FILES} ${SERVICE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/lua)
#target_link_libraries(mse2cdb ${WIN_STD_LIBRARIES} ${WIN_SQLITE3_LIBRARIES} ${WIN_LUA53_LIBRARIES} ${WIN_LIBZIP_LIBRARIES} -static)
target_link_libraries(mse2cdb ${SQLITE3_LIBRARIES} ${LUA53_LIBRARIES} ${LIBZIP_LIBRARIES} ${PTHREAD_LIBRARIES} ${DL_LIBRARIES} ${Z_LIBRARIES} -static)