cmake_minimum_required(VERSION 3.16)
project(mse2cdb VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Release for single-configuration generators when no build type is given.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_VERBOSE_MAKEFILE ON)

option(MSE2CDB_STATIC "Link statically to produce a self-contained portable application" ON)

if(MSE2CDB_STATIC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif()
find_package(sqlite3 REQUIRED)
find_package(Lua53 REQUIRED)
find_package(LibZip REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)

set(MAIN_FILES main.cpp)
set(IO_FILES io/CDBAccess.cpp io/CDBAccess.hpp io/MSEReader.cpp io/MSEReader.hpp io/LuaScriptFinder.cpp io/LuaScriptFinder.hpp)
set(DOMAIN_FILES domain/MSEDataNode.cpp domain/MSEDataNode.hpp domain/CDBCard.hpp)
set(SERVICE_FILES service/LuaCardData.cpp service/LuaCardData.hpp service/LuaMSENodeData.cpp service/LuaMSENodeData.hpp service/LuaConverters.hpp service/LuaConverters.cpp)

add_executable(mse2cdb ${MAIN_FILES} ${IO_FILES} ${DOMAIN_FILES} ${SERVICE_FILES})

target_include_directories(mse2cdb PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SQLITE3_INCLUDE_DIRS}
        ${LUA53_INCLUDE_DIR}
        ${LIBZIP_INCLUDE_DIR})

target_link_libraries(mse2cdb PRIVATE
        ${SQLITE3_LIBRARIES}
        ${LUA53_LIBRARIES}
        ${LIBZIP_LIBRARIES}
        ZLIB::ZLIB
        Threads::Threads)

# GCC 7/8 require a separate library for std::filesystem.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    target_link_libraries(mse2cdb PRIVATE stdc++fs)
endif()

if(MSE2CDB_STATIC)
    target_link_options(mse2cdb PRIVATE -static)
elseif(WIN32)
    message(WARNING "MSE2CDB_STATIC is OFF: the Windows executable will require DLLs at runtime.")
endif()

add_custom_command(TARGET mse2cdb POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/lua $<TARGET_FILE_DIR:mse2cdb>/lua
        COMMENT "Copying Lua scripts next to the executable")