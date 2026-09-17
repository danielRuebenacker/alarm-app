#pragma once
#include "src/interfaces/ISound.h"

class MockSound : public ISound {
  public:
	int ringCount = 0;
	int stopCount = 0;

	void ring() override { ++ringCount; }
	void stopRinging() override { ++stopCount; }
};
