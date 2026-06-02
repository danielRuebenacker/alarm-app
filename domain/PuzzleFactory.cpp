#include "../interfaces/IPuzzle.h"
#include "../types/PuzzleType.h"
#include "../puzzles/MathPuzzle.cpp"

#include <memory>
#include <stdexcept>

class PuzzleFactory {
	public:
		static std::unique_ptr<IPuzzle> createPuzzle(PuzzleType type) {
			switch(type) {
				case PuzzleType::MATHS:
					return std::make_unique<MathPuzzle>();
				case PuzzleType::CAPTCHA:
					return nullptr;
				default:
					throw std::invalid_argument("Unknown puzzle type requested."); 
			} 
		}
};
