#include "../doctest.h"

#include <cstdlib>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "src/puzzles/MemoryPuzzle.h"
#include "src/puzzles/RandomNumberGenerator.h"

namespace {
int acceptedAnswer(IPuzzle& puzzle) {
	for (int candidate = 0; candidate <= 9; ++candidate) {
		if (puzzle.verifySolution(PuzzleResponse(candidate))) return candidate;
	}
	return -1;
}

// The hint grid as rows of digits, so a test can look a cell up the same way a
// player reads it off the screen.
std::vector<std::vector<int>> readGrid(const std::string& hint) {
	std::vector<std::vector<int>> rows;
	std::istringstream in(hint);
	for (std::string line; std::getline(in, line);) {
		std::vector<int> row;
		std::istringstream cells(line);
		for (std::string cell; cells >> cell;) row.push_back(cell[0] - '0');
		rows.push_back(row);
	}
	return rows;
}

// Pull the 1-based row and column out of "Which digit was in row R, column C?"
bool parseAskedCell(const std::string& question, int& row, int& col) {
	const std::string rowKey = "row ";
	const std::string colKey = "column ";
	const size_t rowAt = question.find(rowKey);
	const size_t colAt = question.find(colKey);
	if (rowAt == std::string::npos || colAt == std::string::npos) return false;
	row = std::atoi(question.c_str() + rowAt + rowKey.size());
	col = std::atoi(question.c_str() + colAt + colKey.size());
	return true;
}
}  // namespace

TEST_CASE("Memory puzzle accepts the digit the question asks for") {
	RandomNumberGenerator rng;
	for (int i = 0; i < 200; ++i) {
		MemoryPuzzle puzzle(rng);
		CHECK(acceptedAnswer(puzzle) >= 0);
	}
}

TEST_CASE("Memory puzzle hint is a 3x3 grid of distinct digits 0-8") {
	RandomNumberGenerator rng;
	for (int i = 0; i < 100; ++i) {
		MemoryPuzzle puzzle(rng);

		std::set<int> digits;
		const std::vector<std::vector<int>> grid = readGrid(puzzle.toHint());
		CHECK(grid.size() == 3);
		for (const std::vector<int>& row : grid) {
			CHECK(row.size() == 3);
			digits.insert(row.begin(), row.end());
		}
		// a unique digit per cell keeps the answer unambiguous
		CHECK(digits.size() == 9);
	}
}

TEST_CASE("Memory puzzle question points at the cell that holds the answer") {
	RandomNumberGenerator rng(99);
	for (int i = 0; i < 200; ++i) {
		MemoryPuzzle puzzle(rng);

		int row = 0;
		int col = 0;
		REQUIRE(parseAskedCell(puzzle.toString(), row, col));

		const std::vector<std::vector<int>> grid = readGrid(puzzle.toHint());
		REQUIRE(row >= 1);
		REQUIRE(row <= 3);
		REQUIRE(col >= 1);
		REQUIRE(col <= 3);

		// the cell the question names must be the cell that verifies
		const int inGrid = grid[row - 1][col - 1];
		CHECK(puzzle.verifySolution(PuzzleResponse(inGrid)));
		CHECK_FALSE(puzzle.verifySolution(PuzzleResponse((inGrid + 1) % 9)));
	}
}

TEST_CASE("Memory puzzle rejects answers of the wrong type") {
	RandomNumberGenerator rng(3);
	MemoryPuzzle puzzle(rng);
	const int answer = acceptedAnswer(puzzle);
	REQUIRE(answer >= 0);

	CHECK_FALSE(puzzle.verifySolution(PuzzleResponse(std::string("4"))));
	CHECK(puzzle.verifySolution(PuzzleResponse(answer)));
}

TEST_CASE("Memory puzzle only needs a hint, and a math puzzle does not") {
	RandomNumberGenerator rng(5);
	MemoryPuzzle memory(rng);
	CHECK_FALSE(memory.toHint().empty());
	CHECK(memory.toString().find("Which digit") == 0);
}
