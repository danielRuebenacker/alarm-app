#pragma once
#include <string>

#include "../types/PuzzleResponse.h"

class IPuzzle {
	public:
		virtual ~IPuzzle() = default;
		// the question/prompt shown to the user
		virtual std::string toString() const = 0;
		// get UI touch or terminal input
		virtual bool verifySolution(const PuzzleResponse& userAnswer) = 0;
};
