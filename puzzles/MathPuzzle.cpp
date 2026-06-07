#pragma once
#include "../interfaces/IPuzzle.h"
#include "../types/Difficulty.h"

class MathPuzzle : public IPuzzle {
	private:
		int answer;
	public:
		MathPuzzle(Difficulty difficulty = EASY) {
			// use random num generator or something
		}
		void display(UIWidget* wrapper) override {
			// draw to screen, e.g. 2+2
			// a UI callback updates the userAnswer
		}

		bool verifySolution() override {
			return userAnswer == correctAnswer;
		}
};
