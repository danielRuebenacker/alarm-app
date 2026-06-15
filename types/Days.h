#pragma once
#include <cstdint>

// wrapper struct wraps day mask
struct Days {
	enum Mask : uint8_t {
		Sunday    = 1 << 0, // 00000001
		Monday    = 1 << 1, // 00000010
		Tuesday   = 1 << 2, // 00000100
		Wednesday = 1 << 3, // 00001000
		Thursday  = 1 << 4, // 00010000
		Friday    = 1 << 5, // 00100000
		Saturday  = 1 << 6  // 01000000
	};

	Mask value;

	Days(Mask v) : value(v) {}

	int daysUntilNextActive(int currentDay) const;

	Days operator|(const Days rhs) {
		return static_cast<Mask>(value | rhs.value);
	}

	bool operator&(const Days rhs) {
		return value & rhs.value;
	}
};
