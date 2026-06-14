#pragma once
#include <cstdint>

enum class Days : uint8_t {
    Sunday    = 1 << 0, // 00000001
    Monday    = 1 << 1, // 00000010
    Tuesday   = 1 << 2, // 00000100
    Wednesday = 1 << 3, // 00001000
    Thursday  = 1 << 4, // 00010000
    Friday    = 1 << 5, // 00100000
    Saturday  = 1 << 6  // 01000000
};

inline Days operator|(const Days lhs, const Days rhs) {
	return static_cast<Days>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline bool operator&(const Days lhs, const Days rhs) {
	return static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs);
}
