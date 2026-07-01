cmake_minimum_required(VERSION 3.14)
project(AlarmClockProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_SOURCES
	src/domain/AlarmContext.cpp
	src/domain/Alarm.cpp
	src/domain/AlarmManager.cpp
	src/domain/UIManager.cpp
	src/puzzles/MathPuzzle.h
	src/types/Days.cpp
	src/types/TimePoint.cpp
)

add_library(AlarmClockLib STATIC ${PROJECT_SOURCES})


target_include_directories(AlarmClockLib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
	add_executable(alarm_clock main.cpp)
	target_link_libraries(alarm_clock PRIVATE AlarmClockLib)
endif()

enable_testing()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
	add_subdirectory(tests)
endif()
