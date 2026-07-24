#include <lv/lv.hpp>
#include "src/UI/lvgl/LvglRouter.h"
#include "src/domain/AlarmManager.h"
#include "src/domain/Alarm.h"
#include "src/types/PuzzleType.h"
#include "src/types/Days.h"

#include "tests/mock-interfaces/MockClock.h"
#include "tests/mock-interfaces/MockStorage.h"

#define SCR_WIDTH 320
#define SCR_HEIGHT 480 

int main() {
    lv::init();
    lv::SDLDisplay display(SCR_HEIGHT, SCR_WIDTH);

	MockClock clock;
	MockStorage storage;
	Alarm a1({9, 30}, PuzzleType::MATHS, {});
	storage.addAlarmToStoredAlarms(a1);
	AlarmManager manager(clock, storage);
	manager.getAlarmsFromStorage();

	LvglRouter router(clock, manager);

	router.navigateTo(ScreenType::Home);

    lv::run();
    return 0;
}
