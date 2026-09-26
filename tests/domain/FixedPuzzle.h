#pragma once
#include <string>
#include <utility>

#include "src/interfaces/IPuzzle.h"

// A puzzle with a known answer, for deterministic presenter tests.
class FixedPuzzle : public IPuzzle {
  private:
	int answer_;
	std::string hint_;

  public:
	explicit FixedPuzzle(int answer) : answer_(answer) {}

	// with content the puzzle wants shown briefly, like a memory grid
	FixedPuzzle(int answer, std::string hint)
		: answer_(answer), hint_(std::move(hint)) {}

	std::string toString() const override { return "2 + 3"; }

	std::string toHint() const override { return hint_; }

	bool verifySolution(const PuzzleResponse& userAnswer) override {
		const int* value = std::get_if<int>(&userAnswer.value);
		return value && *value == answer_;
	}
};
