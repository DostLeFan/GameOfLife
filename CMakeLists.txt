cmake_minimum_required(VERSION 3.16)

project(GameOfLife VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

include(FetchContent)

# SFML
message(STATUS "Downloading SFML...")

set(SFML_BUILD_AUDIO TRUE CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS TRUE CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW TRUE CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE FALSE CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS FALSE CACHE BOOL "" FORCE)

FetchContent_Declare(
	SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 3.0.2
	GIT_SHALLOW ON
	EXCLUDE_FROM_ALL
	SYSTEM
)

FetchContent_MakeAvailable(SFML)

message(STATUS "SFML downloaded.")

# TGUI
message(STATUS "Downloading TGUI...")

set(BUILD_SHARED_LIBS OFF)
set(TGUI_BACKEND "SFML_GRAPHICS" CACHE STRING "" FORCE)
set(TGUI_BUILD_EXAMPLES FALSE CACHE BOOL "" FORCE)
set(TGUI_BUILD_TESTS FALSE CACHE BOOL "" FORCE)
set(TGUI_BUILD_GUI_BUILDER FALSE CACHE BOOL "" FORCE)

FetchContent_Declare(
	TGUI
	GIT_REPOSITORY https://github.com/texus/TGUI.git
	GIT_TAG v1.12.0
	GIT_SHALLOW ON
)

FetchContent_MakeAvailable(TGUI)

message(STATUS "TGUI downloaded.")

# TGUI
message(STATUS "Downloading tinyfiledialogs...")

FetchContent_Declare(
	tinyfiledialogs
	URL https://sourceforge.net/projects/tinyfiledialogs/files/tinyfiledialogs-current.zip/download
)

FetchContent_MakeAvailable(tinyfiledialogs)
add_library(tinyfiledialogs STATIC ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c)
target_include_directories(tinyfiledialogs PUBLIC ${tinyfiledialogs_SOURCE_DIR})

message(STATUS "tinyfiledialogs downloaded.")

# nlohmann/json
message(STATUS "Downloading nlohmann/json...")

FetchContent_Declare(
	json
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG v3.12.0
	GIT_SHALLOW ON
)

FetchContent_MakeAvailable(json)

message(STATUS "nlohmann/json downloaded.")

# spdlog
message(STATUS "Downloading spdlog...")

set(SPDLOG_BUILD_ASYNC ON CACHE BOOL "" FORCE)
# ou selon ta version :
set(SPDLOG_ENABLE_ASYNC ON CACHE BOOL "" FORCE)

FetchContent_Declare(
	spdlog
	GIT_REPOSITORY https://github.com/gabime/spdlog.git
	GIT_TAG v1.17.0
	GIT_SHALLOW ON
)

FetchContent_MakeAvailable(spdlog)

message(STATUS "spdlog downloaded.")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "include/*.hpp")

add_executable(${CMAKE_PROJECT_NAME} ${HEADERS} ${SOURCES})
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System TGUI::TGUI tinyfiledialogs nlohmann_json::nlohmann_json spdlog::spdlog_header_only)

# On Windows : avoid console in release mode.
# if(WIN32)
	# set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
# endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})