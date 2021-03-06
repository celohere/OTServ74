cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(otserv)

# Additional CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Revscriptsys2
set(VERSION "0.7.0")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -DDEBUG")

# add debug definition
if(CMAKE_BUILD_TYPE EQUAL "Debug")
	add_definitions(-D__DEBUG__ -D__DEBUG_NET_DETAIL__ -D__DEBUG__MOVESYS__ -D__DEBUG_CRITICALSECTION__ -D__DEBUG_HOUSES__ -D__DEBUG_PLAYERS__ -D__DEBUG_SCHEDULER__ -D__DEBUG_SCRIPT_ENVIRONMENT_OBJECTMAP__)
endif()

# Options
option(USE_MYSQL "Use MySQL" OFF)
option(USE_SQLITE "Use SQLite" OFF)
option(USE_LUAJIT "Use LuaJIT" OFF)
option(USE_DIAGNOSTIC "Use server diagnostic" OFF)
option(USE_SKULLSYSTEM "Skull system" ON)
option(USE_STATIC_LIBS "Static linking" OFF)

# Status
message(STATUS "MySQL: " ${USE_MYSQL})
message(STATUS "SQLite: " ${USE_SQLITE})
message(STATUS "LuaJIT: " ${USE_LUAJIT})

message(STATUS "Server diagnostic: " ${USE_DIAGNOSTIC})
message(STATUS "Skull system: " ${USE_SKULLSYSTEM})
message(STATUS "Static libraries: " ${USE_STATIC_LIBS})

# Make sure at least one database driver is selected
if(NOT USE_MYSQL AND NOT USE_SQLITE)
	message(FATAL_ERROR "At least one database driver has to be selected.")
endif()

# Sources
add_subdirectory(Source)
