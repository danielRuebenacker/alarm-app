#pragma once
#include <string>

#include "src/interfaces/IPuzzle.h"

// A puzzle with a known answer, for deterministic presenter tests.
class FixedPuzzle : public IPuzzle {
  private:
	int answer_;

  public:
	explicit FixedPuzzle(int answer) : answer_(answer) {}

	std::string toString() const override { return "2 + 3"; }

	bool verifySolution(const PuzzleResponse& userAnswer) override {
		const int* value = std::get_if<int>(&userAnswer.value);
		return value && *value == answer_;
	}
};
