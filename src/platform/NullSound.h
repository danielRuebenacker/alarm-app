#pragma once
#include "../interfaces/ISound.h"

// Silent ISound used on platforms without an audio backend (e.g. the desktop
// SDL build). A real backend (SDL audio, M5Stack buzzer, ...) just implements
// ISound and is handed to AlarmManager::setSound().
class NullSound : public ISound {
  public:
	void ring() override {}
	void stopRinging() override {}
};
