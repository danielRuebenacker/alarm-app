#pragma once
#include "../types/ScreenType.h"

class IRouter {
    public:
        virtual ~IRouter() = default;
        // allow for passing optional alarm id
        // this method handles specific routing (creation/deletion of screens + hooking up with presenter)
        virtual void navigateTo(ScreenType screen, int targetAlarmId = -1) = 0;
};
