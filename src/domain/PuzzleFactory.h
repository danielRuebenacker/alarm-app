#pragma once
#include "../interfaces/IPuzzle.h"
#include "../types/PuzzleType.h"
#include "../puzzles/CaptchaPuzzle.h"
#include "../puzzles/MathPuzzle.h"
#include "../puzzles/MemoryPuzzle.h"

#include <memory>
#include <stdexcept>

class PuzzleFactory {
    RandomNumberGenerator& rd_;

	public:
	PuzzleFactory(RandomNumberGenerator& rd) : rd_(rd) {}
		virtual ~PuzzleFactory() = default;

		virtual std::unique_ptr<IPuzzle> createPuzzle(PuzzleType type) {
			switch(type) {
				case PuzzleType::MATHS:
					return std::make_unique<EasyMathPuzzle>(rd_);
				case PuzzleType::MEMORY:
					return std::make_unique<MemoryPuzzle>(rd_);
				case PuzzleType::CAPTCHA:
					return std::make_unique<CaptchaPuzzle>(rd_);
				default:
					throw std::invalid_argument("Unknown puzzle type requested.");
			} 
		}
};
