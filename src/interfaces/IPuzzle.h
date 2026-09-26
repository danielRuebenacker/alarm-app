#pragma once
#include <string>

#include "../types/PuzzleResponse.h"

class IPuzzle {
	public:
		virtual ~IPuzzle() = default;
		// the question/prompt shown to the user
		virtual std::string toString() const = 0;
		// extra content the user has to look at before answering, shown only
		// briefly (e.g. the grid a memory puzzle wants memorised).
		// Empty for puzzles that need no preparation.
		virtual std::string toHint() const { return {}; }
		// get UI touch or terminal input
		virtual bool verifySolution(const PuzzleResponse& userAnswer) = 0;
};
