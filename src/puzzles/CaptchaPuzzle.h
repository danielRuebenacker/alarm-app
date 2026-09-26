#pragma once
#include <cctype>
#include <iterator>
#include <string>

#include "../interfaces/IPuzzle.h"
#include "RandomNumberGenerator.h"

// "Type the word you see". There is no image renderer here, so the challenge is
// a word in inconsistent casing rather than a distorted bitmap - enough to make
// the user read it properly instead of pattern matching. Checking is
// case-insensitive, so the casing is presentation, not part of the answer.
class CaptchaPuzzle : public IPuzzle {
  private:
	// deliberately short and easy to read, not real captcha entropy
	inline static const char* const kWords[] = {
		"ALARM", "SNOOZE", "MORNING", "BEDTIME", "WAKE", "CLOCK",
		"SILENT", "DOZE", "DAYTIME", "SHINE", "ROOSTER", "THUNDER",
	};
	inline static constexpr int kWordCount = static_cast<int>(std::size(kWords));

	std::string challenge_;

	// upper or lower each letter at random, leaving the digits alone
	static std::string scramble(const std::string& word, RandomNumberGenerator& rd) {
		std::string out;
		out.reserve(word.size());
		for (const char c : word) {
			const bool upper = rd.generateRandomNumber(0, 1) == 1;
			out += static_cast<char>(upper ? std::toupper(static_cast<unsigned char>(c))
										   : std::tolower(static_cast<unsigned char>(c)));
		}
		return out;
	}

	static std::string normalise(const std::string& text) {
		std::string out;
		out.reserve(text.size());
		for (const char c : text) {
			// ignore casing and any surrounding whitespace the user typed
			if (std::isspace(static_cast<unsigned char>(c))) continue;
			out += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
		}
		return out;
	}

  public:
	explicit CaptchaPuzzle(RandomNumberGenerator& rd) {
		challenge_ = scramble(kWords[rd.generateRandomNumber(0, kWordCount - 1)], rd);
	}

	std::string toString() const override { return "Type the word: " + challenge_; }

	bool verifySolution(const PuzzleResponse& userAnswer) override {
		// the answer is text, so it has to arrive as a string
		const std::string* val = std::get_if<std::string>(&userAnswer.value);
		return val && normalise(*val) == normalise(challenge_);
	}
};
