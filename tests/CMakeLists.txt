add_executable(run_project_tests
	test_main.cpp
	types/DaysTest.cpp
	types/TimePointTest.cpp
	domain/AlarmTest.cpp
  domain/AlarmManagerTest.cpp
  domain/MockAlarm.cpp
)

target_link_libraries(run_project_tests PRIVATE AlarmClockLib)

add_test(NAME CoreUnitTests COMMAND run_project_tests)
