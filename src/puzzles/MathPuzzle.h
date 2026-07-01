#pragma once
#include "../interfaces/IPuzzle.h"
#include "../types/Difficulty.h"
#include "RandomNumberGenerator.h"
#include <cmath>

class MathPuzzle : public IPuzzle {
    public:
        virtual std::string toString() const = 0;

        bool verifySolution(const PuzzleResponse &userAnswer) override {
            // get const pointer to value
            const int* val = std::get_if<int>(&userAnswer.value);
            // dereference
            if (*val == correctAnswer) {
                return true;
            }
            return false;
        }
    protected:
        int correctAnswer;
};

class EasyMathPuzzle : public MathPuzzle {
	private:
		int x, y, z;
		RandomNumberGenerator& rd_;

	public:
		void display(UIWidget* wrapper) override {}

		bool verifySolution() override { 
			return true;
		}
};
