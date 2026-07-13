#pragma once
#include "src/domain/Alarm.h"

Alarm createMockAlarm(int hour, int min, const Days& schedule = Days{});
