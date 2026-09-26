#pragma once
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

#include "../interfaces/IPuzzle.h"
#include "RandomNumberGenerator.h"

// Shows a grid of digits for a few seconds, then asks which one was in a
// particular cell. The grid is a permutation of 0-8 so no digit repeats, which
// makes it easier to hold on to and removes any ambiguity about the answer.
class MemoryPuzzle : public IPuzzle {
  private:
	static constexpr int kSize = 3;

	int grid_[kSize][kSize];
	int row_;
	int col_;

  public:
	explicit MemoryPuzzle(RandomNumberGenerator& rd) {
		// digits 0..8, then a Fisher-Yates shuffle so every cell is distinct
		int digits[kSize * kSize];
		std::iota(std::begin(digits), std::end(digits), 0);
		for (int i = kSize * kSize - 1; i > 0; --i) {
			std::swap(digits[i], digits[rd.generateRandomNumber(0, i)]);
		}
		for (int r = 0; r < kSize; ++r) {
			for (int c = 0; c < kSize; ++c) {
				grid_[r][c] = digits[r * kSize + c];
			}
		}
		row_ = rd.generateRandomNumber(0, kSize - 1);
		col_ = rd.generateRandomNumber(0, kSize - 1);
	}

	std::string toString() const override {
		// rows and columns are numbered from 1 so they read like the hint grid
		return "Which digit was in row " + std::to_string(row_ + 1) + ", column " +
			   std::to_string(col_ + 1) + "?";
	}

	std::string toHint() const override {
		std::string grid;
		for (int r = 0; r < kSize; ++r) {
			for (int c = 0; c < kSize; ++c) {
				if (c > 0) grid += "   ";
				grid += std::to_string(grid_[r][c]);
			}
			if (r < kSize - 1) grid += "\n";
		}
		return grid;
	}

	bool verifySolution(const PuzzleResponse& userAnswer) override {
		// the answer is a single digit, so it has to arrive as an int
		const int* val = std::get_if<int>(&userAnswer.value);
		return val && *val == grid_[row_][col_];
	}
};
