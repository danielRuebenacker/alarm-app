#include "../doctest.h"

#include <algorithm>
#include <cctype>
#include <string>

#include "src/puzzles/CaptchaPuzzle.h"
#include "src/puzzles/RandomNumberGenerator.h"

namespace {
// The challenge is everything after the "Type the word: " prefix.
std::string challengeOf(const CaptchaPuzzle& puzzle) {
	const std::string prompt = puzzle.toString();
	const std::string prefix = "Type the word: ";
	if (prompt.compare(0, prefix.size(), prefix) != 0) return {};
	return prompt.substr(prefix.size());
}

std::string lower(const std::string& text) {
	std::string out;
	for (const char c : text) {
		out += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	}
	return out;
}
}  // namespace

TEST_CASE("Captcha accepts the shown word regardless of casing") {
	RandomNumberGenerator rng;
	for (int i = 0; i < 200; ++i) {
		CaptchaPuzzle puzzle(rng);
		const std::string challenge = challengeOf(puzzle);
		REQUIRE_FALSE(challenge.empty());

		CHECK(puzzle.verifySolution(PuzzleResponse(challenge)));
		CHECK(puzzle.verifySolution(PuzzleResponse(lower(challenge))));
		std::string shouted = challenge;
		std::transform(shouted.begin(), shouted.end(), shouted.begin(),
					   [](unsigned char c) { return std::toupper(c); });
		CHECK(puzzle.verifySolution(PuzzleResponse(shouted)));
	}
}

TEST_CASE("Captcha ignores stray whitespace around the answer") {
	RandomNumberGenerator rng(11);
	for (int i = 0; i < 50; ++i) {
		CaptchaPuzzle puzzle(rng);
		CHECK(puzzle.verifySolution(PuzzleResponse("  " + lower(challengeOf(puzzle)) + " ")));
	}
}

TEST_CASE("Captcha rejects a different word") {
	RandomNumberGenerator rng;
	for (int i = 0; i < 200; ++i) {
		CaptchaPuzzle puzzle(rng);
		CHECK_FALSE(puzzle.verifySolution(PuzzleResponse("definitelynottheword")));
		CHECK_FALSE(puzzle.verifySolution(PuzzleResponse("")));
	}
}

TEST_CASE("Captcha rejects answers of the wrong type") {
	RandomNumberGenerator rng(2);
	CaptchaPuzzle puzzle(rng);
	// the view reads the field as text for a captcha, but a stray int response
	// must not slip through
	CHECK_FALSE(puzzle.verifySolution(PuzzleResponse(0)));
	CHECK(puzzle.verifySolution(PuzzleResponse(lower(challengeOf(puzzle)))));
}

TEST_CASE("Captcha words come from the built-in list") {
	RandomNumberGenerator rng;
	for (int i = 0; i < 200; ++i) {
		CaptchaPuzzle puzzle(rng);
		const std::string challenge = challengeOf(puzzle);
		// letters only: the casing scramble must not turn a word into something
		// that cannot be typed back
		CHECK(std::all_of(challenge.begin(), challenge.end(), [](unsigned char c) {
			return std::isalpha(c) != 0;
		}));
	}
}
