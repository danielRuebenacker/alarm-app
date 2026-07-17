#include "../doctest.h"

#include "../mock-interfaces/MockClock.h"
#include "../mock-interfaces/MockStorage.h"

#include "src/domain/AlarmManager.h"
#include "MockAlarm.h"


TEST_CASE("AlarmManager basic functionality tests") {
    MockClock mockClock;
    MockStorage mockStorage;
    AlarmManager manager(mockClock, mockStorage);

    Alarm::setNextId(1);
    Alarm a1 = createMockAlarm(6, 0);		a1.turnOn();
    Alarm a2 = createMockAlarm(9, 30);	a2.turnOn();

    SUBCASE("setting alarms: adding new alarms and overwriting existing ones") {
        manager.addAlarm(a1);
        CHECK(manager.getAlarms().size() == 1);
        // check this also impacted the stored alarms! (i.e. we saved the alarm after setting)
        CHECK(mockStorage.loadAlarms().size() == 1);
        CHECK(mockStorage.loadAlarms()[0].getId() == 1);

        // overwriting
        a1.setTime(7, 30);
        manager.addAlarm(a1);
        // check its still only the one alarm
        CHECK(manager.getAlarms().size() == 1);
        CHECK(mockStorage.loadAlarms().size() == 1);
        CHECK(mockStorage.loadAlarms()[0].getId() == 1);
        Alarm returnedAlarm = manager.getAlarms()[0];
        CHECK(returnedAlarm.getTime().hour() == 7);
        CHECK(returnedAlarm.getTime().min() == 30);
    }

    SUBCASE("Loading alarms and tracking nextId") {
        mockStorage.addAlarmToStoredAlarms(a1);
        mockStorage.addAlarmToStoredAlarms(a2);

        manager.getAlarmsFromStorage();
        CHECK(manager.getAlarms().size() == 2);

        Alarm newAutoAlarm = createMockAlarm(12, 0);
        CHECK(newAutoAlarm.getId() == 3);
    }

    SUBCASE("Deleting alarms (existent and non-existent)") {
        manager.addAlarm(a1);
        manager.addAlarm(a2);
        REQUIRE(manager.getAlarms().size() == 2);

        // deleting bad Id shouldn't do anything
        manager.deleteAlarm(999);
        CHECK(manager.getAlarms().size() == 2);

        // deleting real alarm
        manager.deleteAlarm(1);
        auto remaining = manager.getAlarms();
        REQUIRE(remaining.size() == 1);
        // check it actually deleted the right alarm (other has correct Id)
        CHECK(remaining[0].getId() == 2);
    }
}

TEST_CASE("Test getNextActiveAlarm") {
    MockClock mockClock;
    MockStorage mockStorage;
    AlarmManager manager(mockClock, mockStorage);

    // set the date/time
    mockClock.setTime(8, 0);
    mockClock.setCurrentDay(Days::Monday);

    SUBCASE("Test with inactive alarm -- should return nullptr") {
      Alarm off = createMockAlarm(0, 0);
	  manager.addAlarm(off);
	  const Alarm *next = manager.getNextActiveAlarm();
	  REQUIRE(next == nullptr);
    }

    SUBCASE("Test with three simple alarms, no day masks, also check dismissing alarms") {
        // known Ids
        Alarm::setNextId(10);
        Alarm earlyAlarm = createMockAlarm(6, 0);  earlyAlarm.turnOn();  // Id = 10
        Alarm targetAlarm = createMockAlarm(9, 0); targetAlarm.turnOn(); // Id = 11
        Alarm farAlarm = createMockAlarm(11, 0);   farAlarm.turnOn();    // Id = 12

        manager.addAlarm(earlyAlarm);
        manager.addAlarm(targetAlarm);
        manager.addAlarm(farAlarm);

        // closest alarm should be the target alarm
        const Alarm* next = manager.getNextActiveAlarm();
        REQUIRE(next != nullptr);
        CHECK(next->getId() == 11);

        // dismissing should now return the far alarm!
        manager.dismissAlarm(11);
        next = manager.getNextActiveAlarm();
        REQUIRE(next != nullptr);
        CHECK(next->getId() == 12);

        // same with turning off now it should be the early alarm
        farAlarm.turnOff();
        manager.addAlarm(farAlarm); // update!!

        next = manager.getNextActiveAlarm();
        REQUIRE(next != nullptr);
        CHECK(next->getId() == 10); // wrapping
    }

    SUBCASE("Test with complex alarms (with days set)") {
		// recall time is Mon :: 8 am
		Alarm::setNextId(20);
		Alarm monTuesEvening = createMockAlarm(18, 0);  monTuesEvening.turnOn();  // Id = 20
		monTuesEvening.setDay(Days::Monday);
		monTuesEvening.setDay(Days::Tuesday);

		Alarm tuesWedMorning = createMockAlarm(9, 0); tuesWedMorning.turnOn(); // Id = 21
		tuesWedMorning.setDay(Days::Tuesday);
		tuesWedMorning.setDay(Days::Wednesday);

		manager.addAlarm(monTuesEvening);
		manager.addAlarm(tuesWedMorning);

		// basic checking that mon evening is next
		const Alarm* next = manager.getNextActiveAlarm();
		REQUIRE(next != nullptr);
		CHECK(next->getId() == monTuesEvening.getId());

		// check when dismissing it its now tuesday morning
		manager.dismissAlarm(monTuesEvening.getId());
		next = manager.getNextActiveAlarm();
		REQUIRE(next != nullptr);
		CHECK(next->getId() == tuesWedMorning.getId());
		
		// dismiss this alarm also (should then loop back around to Tuesday evening)
		manager.dismissAlarm(tuesWedMorning.getId());
		next = manager.getNextActiveAlarm();
		REQUIRE(next != nullptr);
		CHECK(next->getId() == monTuesEvening.getId());
    }
}
