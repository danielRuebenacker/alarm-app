#pragma once
#include "../interfaces/IPuzzle.h"
#include "../types/PuzzleType.h"
#include "../puzzles/MathPuzzle.h"

#include <memory>
#include <stdexcept>

class PuzzleFactory {
    RandomNumberGenerator& rd_;

	public:
        PuzzleFactory(RandomNumberGenerator& rd) : rd_(rd) {}

		std::unique_ptr<IPuzzle> createPuzzle(PuzzleType type) {
			switch(type) {
				case PuzzleType::MATHS:
					return std::make_unique<EasyMathPuzzle>(rd_);
				case PuzzleType::CAPTCHA:
					return nullptr;
				default:
					throw std::invalid_argument("Unknown puzzle type requested."); 
			} 
		}
};
