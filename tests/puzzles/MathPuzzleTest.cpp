#include "../doctest.h"

#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#include "src/domain/PuzzleFactory.h"
#include "src/puzzles/MathPuzzle.h"
#include "src/puzzles/RandomNumberGenerator.h"

namespace {
// Evaluate the prompt exactly as the player reads it: * and ^ bind tighter
// than + and -. Deliberately re-derives the arithmetic from the string rather
// than trusting the generator, so it catches a prompt and an accepted answer
// that have drifted apart.
int evaluatePrompt(const std::string& prompt) {
	std::istringstream in(prompt);
	std::vector<std::string> tokens;
	for (std::string token; in >> token;) {
		// the hard variant prints "14^2" with no spaces around the exponent
		const size_t caret = token.find('^');
		if (caret == std::string::npos) {
			tokens.push_back(token);
		} else {
			tokens.push_back(token.substr(0, caret));
			tokens.push_back("^");
			tokens.push_back(token.substr(caret + 1));
		}
	}

	REQUIRE(tokens.size() >= 3);
	REQUIRE((tokens.size() % 2) == 1);

	int total = 0;
	int term = std::stoi(tokens[0]);
	std::string pending = "+";
	for (size_t i = 1; i < tokens.size(); i += 2) {
		const std::string& op = tokens[i];
		const int operand = std::stoi(tokens[i + 1]);
		if (op == "*") {
			term *= operand;
		} else if (op == "^") {
			term = static_cast<int>(std::lround(std::pow(term, operand)));
		} else {
			total += (pending == "-" ? -term : term);
			pending = op;
			term = operand;
		}
	}
	return total + (pending == "-" ? -term : term);
}

// The accepted answer is private to the puzzle, so find it the only way a
// player can: by trying candidates.
int acceptedAnswer(IPuzzle& puzzle, int lo, int hi) {
	for (int candidate = lo; candidate <= hi; ++candidate) {
		if (puzzle.verifySolution(PuzzleResponse(candidate))) return candidate;
	}
	return hi + 1;  // sentinel: no candidate in range was accepted
}
}  // namespace

TEST_CASE("Math prompts state the arithmetic that is actually accepted") {
	// each difficulty has its own operand range, so search the widest one
	RandomNumberGenerator rng;
	for (int i = 0; i < 300; ++i) {
		EasyMathPuzzle easy(rng);
		const int answer = acceptedAnswer(easy, -200, 400);
		CHECK(evaluatePrompt(easy.toString()) == answer);
	}

	for (int i = 0; i < 300; ++i) {
		MediumMathPuzzle medium(rng);
		const int answer = acceptedAnswer(medium, -200, 1400);
		CHECK(evaluatePrompt(medium.toString()) == answer);
	}

	for (int i = 0; i < 300; ++i) {
		HardMathPuzzle hard(rng);
		const int answer = acceptedAnswer(hard, -400, 600);
		CHECK(evaluatePrompt(hard.toString()) == answer);
	}
}

TEST_CASE("A sum of three two digit numbers is accepted") {
	// the exact case from the bug report: 61 + 73 + 76 == 210
	RandomNumberGenerator rng;
	EasyMathPuzzle puzzle(rng);

	const int answer = acceptedAnswer(puzzle, -200, 400);
	CHECK(evaluatePrompt(puzzle.toString()) == answer);
	CHECK(puzzle.verifySolution(PuzzleResponse(answer)));
	CHECK_FALSE(puzzle.verifySolution(PuzzleResponse(answer + 1)));
	CHECK_FALSE(puzzle.verifySolution(PuzzleResponse(answer - 1)));
}

TEST_CASE("Math puzzles reject answers of the wrong type") {
	RandomNumberGenerator rng;
	EasyMathPuzzle puzzle(rng);
	const int answer = acceptedAnswer(puzzle, -200, 400);

	// the view hands back whatever it parsed, so a non-numeric field must not
	// be able to satisfy the check
	CHECK_FALSE(puzzle.verifySolution(PuzzleResponse(std::string("210"))));
	CHECK_FALSE(puzzle.verifySolution(PuzzleResponse(std::vector<std::vector<int>>{})));
	CHECK(puzzle.verifySolution(PuzzleResponse(answer)));
}

TEST_CASE("HardMathPuzzle does not lose precision through std::pow") {
	// std::pow returns a double; squaring has to stay exact for the prompt and
	// the accepted answer to agree
	RandomNumberGenerator rng(12345);
	for (int i = 0; i < 300; ++i) {
		HardMathPuzzle hard(rng);
		CHECK(evaluatePrompt(hard.toString()) == acceptedAnswer(hard, -400, 600));
	}
}

TEST_CASE("PuzzleFactory builds every selectable puzzle type") {
	RandomNumberGenerator rng(7);
	PuzzleFactory factory(rng);

	for (const PuzzleType type : {PuzzleType::MATHS, PuzzleType::MEMORY, PuzzleType::CAPTCHA}) {
		std::unique_ptr<IPuzzle> puzzle;
		REQUIRE_NOTHROW(puzzle = factory.createPuzzle(type));
		// a factory that hands back nothing used to look like a solved puzzle
		REQUIRE(puzzle != nullptr);
		CHECK_FALSE(puzzle->toString().empty());
	}
}
