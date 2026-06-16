#pragma once
#include "../interfaces/IAppState.h"
#include "../interfaces/IClock.h"
#include "../domain/Alarm.h"

class PrimedState : public IAppState {
	public:
		void update(AlarmContext* context) override {
			// check whether alarm time exceeded
			TimePoint now = context->getClock().now();
			// Alarm* nextAlarm = context->
		}
};
