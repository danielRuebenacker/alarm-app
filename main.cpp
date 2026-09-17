#include <lv/lv.hpp>
#include "src/UI/lvgl/LvglRouter.h"
#include "src/UI/lvgl/LvglScheduler.h"
#include "src/domain/AlarmManager.h"
#include "src/domain/AlarmScheduler.h"
#include "src/domain/Alarm.h"
#include "src/domain/PuzzleFactory.h"
#include "src/puzzles/RandomNumberGenerator.h"
#include "src/types/PuzzleType.h"
#include "src/types/Days.h"

#include "src/platform/SystemClock.h"
#include "tests/mock-interfaces/MockStorage.h"

#define SCR_WIDTH 320
#define SCR_HEIGHT 480 

int main() {
    lv::init();
    lv::SDLDisplay display(SCR_HEIGHT, SCR_WIDTH);

	SystemClock clock;
	MockStorage storage;
	Alarm a1({9, 30}, PuzzleType::MATHS, {});
	a1.turnOn();
	storage.addAlarmToStoredAlarms(a1);
	AlarmManager manager(clock, storage);
	manager.getAlarmsFromStorage();
	manager.getDismissedAlarmIdsFromStorage();

	LvglScheduler scheduler;
	RandomNumberGenerator rng;
	PuzzleFactory puzzleFactory(rng);
	LvglRouter router(clock, manager, scheduler, puzzleFactory);

	AlarmScheduler alarmScheduler(manager, clock, scheduler);
	// a due alarm opens the ringing screen; it stays armed until resolved
	alarmScheduler.setOnAlarmDue([&router](const Alarm& alarm) {
		router.navigateTo(ScreenType::Ringing, alarm.getId());
	});
	alarmScheduler.start();

	router.navigateTo(ScreenType::Home);

    lv::run();
    return 0;
}
