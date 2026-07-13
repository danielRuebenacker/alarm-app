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

    SUBCASE("Test with three simple alarms, no day masks, also check dismissing alarms") {
        // set the date/time
        mockClock.setTime(8, 0);
        mockClock.setCurrentDay(Days::Monday);

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
}
