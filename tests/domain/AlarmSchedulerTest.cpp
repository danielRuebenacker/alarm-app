#include "../doctest.h"

#include <chrono>

#include "../mock-interfaces/MockClock.h"
#include "../mock-interfaces/MockScheduler.h"
#include "../mock-interfaces/MockStorage.h"

#include "src/domain/AlarmManager.h"
#include "src/domain/AlarmScheduler.h"
#include "MockAlarm.h"

TEST_CASE("AlarmScheduler arms a timer for the next active alarm") {
	MockClock clock;
	MockStorage storage;
	MockScheduler scheduler;

	clock.setTime(8, 0);
	clock.setCurrentDay(Days::Monday);

	AlarmManager manager(clock, storage);
	Alarm alarm = createMockAlarm(9, 0);
	alarm.turnOn();
	manager.addAlarm(alarm);

	AlarmScheduler alarmScheduler(manager, clock, scheduler);
	alarmScheduler.start();

	CHECK(scheduler.pendingCount() == 1);
	CHECK(scheduler.nextDelay() == std::chrono::minutes(60));
}

TEST_CASE("AlarmScheduler fires a due alarm") {
	MockClock clock;
	MockStorage storage;
	MockScheduler scheduler;

	clock.setTime(9, 0);
	clock.setCurrentDay(Days::Monday);

	AlarmManager manager(clock, storage);
	Alarm alarm = createMockAlarm(9, 0);
	alarm.turnOn();
	manager.addAlarm(alarm);

	AlarmScheduler alarmScheduler(manager, clock, scheduler);

	bool fired = false;
	int firedId = -1;
	alarmScheduler.setOnAlarmDue([&](const Alarm& due) {
		fired = true;
		firedId = due.getId();
		manager.dismissAlarm(due.getId());
	});
	alarmScheduler.start();

	REQUIRE(scheduler.pendingCount() == 1);
	CHECK(scheduler.nextDelay() == std::chrono::minutes(0));

	scheduler.fireNext();

	CHECK(fired);
	CHECK(firedId == alarm.getId());
	// dismissed for the day, so the next ring is ~24h away
	CHECK(scheduler.pendingCount() == 1);
	CHECK(scheduler.nextDelay() == std::chrono::minutes(24 * 60));
}

TEST_CASE("AlarmScheduler re-arms when alarm data changes") {
	MockClock clock;
	MockStorage storage;
	MockScheduler scheduler;

	clock.setTime(8, 0);
	clock.setCurrentDay(Days::Monday);

	AlarmManager manager(clock, storage);
	Alarm later = createMockAlarm(9, 0);
	later.turnOn();
	Alarm earlier = createMockAlarm(8, 30);

	manager.addAlarm(later);
	manager.addAlarm(earlier);

	AlarmScheduler alarmScheduler(manager, clock, scheduler);
	alarmScheduler.start();

	// only `later` is active
	CHECK(scheduler.nextDelay() == std::chrono::minutes(60));

	// activating the earlier alarm should cause a re-arm
	manager.toggleAlarm(earlier.getId());
	CHECK(scheduler.pendingCount() == 1);
	CHECK(scheduler.nextDelay() == std::chrono::minutes(30));
}

TEST_CASE("AlarmScheduler keeps a ringing alarm from re-firing") {
	MockClock clock;
	MockStorage storage;
	MockScheduler scheduler;

	clock.setTime(9, 0);
	clock.setCurrentDay(Days::Monday);
	clock.setDaysSince1970(100);

	AlarmManager manager(clock, storage);
	Alarm alarm = createMockAlarm(9, 0);
	alarm.turnOn();
	alarm.setSnoozeMinutes(5);
	manager.addAlarm(alarm);

	AlarmScheduler alarmScheduler(manager, clock, scheduler);
	int firedCount = 0;
	// deliberately does not dismiss: the alarm stays ringing
	alarmScheduler.setOnAlarmDue([&](const Alarm&) { ++firedCount; });
	alarmScheduler.start();

	REQUIRE(scheduler.pendingCount() == 1);
	scheduler.fireNext();

	CHECK(firedCount == 1);
	CHECK(manager.isRinging());
	// a ringing alarm must not be re-armed
	CHECK(scheduler.pendingCount() == 0);

	// snoozing arms a one-off timer for the snooze duration
	REQUIRE(manager.snoozeAlarm(alarm.getId()));
	REQUIRE(scheduler.pendingCount() == 1);
	CHECK(scheduler.nextDelay() == std::chrono::minutes(5));

	// firing the snooze rings the alarm again
	scheduler.fireNext();
	CHECK(firedCount == 2);
	CHECK(manager.isRinging());
}
