cmake_minimum_required(VERSION 3.14)
project(AlarmClockProject LANGUAGES CXX C) # C for lvgl

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED) # for UI

set(LV_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h CACHE STRING "" FORCE)

# disable build examples
set(LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)

# tell lvgl to use SDL
set(LV_USE_SDL 1 CACHE BOOL "" FORCE)

# include lvgl and lv
add_subdirectory(external/lvgl)
add_subdirectory(external/lv)

file(GLOB_RECURSE PROJECT_SOURCES
	"src/*.cpp" "src/*.h"
)

add_library(AlarmClockLib STATIC ${PROJECT_SOURCES})

# link to lib target
target_link_libraries(AlarmClockLib PUBLIC lv)

# allow include headers cleanly
target_include_directories(AlarmClockLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
	add_executable(alarm_clock main.cpp)

	target_link_libraries(alarm_clock PRIVATE
		AlarmClockLib
		SDL2::SDL2
	)

	target_include_directories(alarm_clock PRIVATE
		${SDL2_INCLUDE_DIRS}
	)
endif()

enable_testing()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
	add_subdirectory(tests)
endif()
