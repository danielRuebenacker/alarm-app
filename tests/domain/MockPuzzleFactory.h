#pragma once
#include <memory>

#include "src/domain/PuzzleFactory.h"

// Hands out a pre-built puzzle so tests are deterministic. A null puzzle stands
// in for a type the factory cannot build.
class MockPuzzleFactory : public PuzzleFactory {
  private:
	std::unique_ptr<IPuzzle> puzzle_;

  public:
	MockPuzzleFactory(std::unique_ptr<IPuzzle> puzzle, RandomNumberGenerator& rd)
		: PuzzleFactory(rd), puzzle_(std::move(puzzle)) {}

	std::unique_ptr<IPuzzle> createPuzzle(PuzzleType) override {
		return std::move(puzzle_);
	}
};
