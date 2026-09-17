#pragma once
#include <vector>

#include "src/interfaces/IRouter.h"

class MockRouter : public IRouter {
  public:
	struct Navigation {
		ScreenType screen;
		int alarmId;
	};

	std::vector<Navigation> navigations;

	void navigateTo(ScreenType screen, int targetAlarmId = -1) override {
		navigations.push_back({screen, targetAlarmId});
	}

	bool navigatedTo(ScreenType screen) const {
		for (const auto& nav : navigations) {
			if (nav.screen == screen) return true;
		}
		return false;
	}

	ScreenType lastScreen() const { return navigations.back().screen; }
};
