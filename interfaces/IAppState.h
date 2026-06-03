#pragma once
#include "../domain/AlarmContext.h"

class AlarmContext;

class IAppState {
	public:
		virtual ~IAppState() = default;
		virtual void update(AlarmContext* context) = 0;
};
