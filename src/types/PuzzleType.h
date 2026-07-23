#pragma once
#include <string_view>

enum PuzzleType {
	MATHS,
	MEMORY,
	CAPTCHA
};

constexpr std::string_view to_string(PuzzleType type) {
    switch (type) {
        case PuzzleType::MATHS:   return "MATHS";
        case PuzzleType::MEMORY:  return "MEMORY";
        case PuzzleType::CAPTCHA: return "CAPTCHA";
        default:                  return "UNKNOWN";
    }
}
