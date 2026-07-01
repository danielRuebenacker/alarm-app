#pragma once
#include "../types/PuzzleResponse.h"

class IPuzzle {
	public:
		virtual ~IPuzzle() = default;
		// get UI touch or terminal input
		virtual bool verifySolution(const PuzzleResponse& userAnswer);
};
