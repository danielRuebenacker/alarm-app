#pragma once
#include "../interfaces/IPuzzle.h"

class MathPuzzle : public IPuzzle {
	private:
		int answer;
	public:
		MathPuzzle() {
			// use random num generator or something
			answer = 0;
		}
		void display(const IScreen& wrapper) override {
			// draw to screen, e.g. 2+2
		}

		bool verifySolution() override {
			// get number input (e.g. from keypad), verify solution
			// don't worry about how to get, will use UI input
			return false;
		}
};
