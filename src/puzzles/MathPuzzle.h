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
		EasyMathPuzzle(RandomNumberGenerator& rd) : rd_(rd) {
			x = rd_.generateRandomNumber(1, 100);
			y = rd_.generateRandomNumber(1, 100);
			z = rd_.generateRandomNumber(1, 100);
			correctAnswer = x + y + z;
		}

		std::string toString() const override {
			return std::to_string(x) + " + " + std::to_string(y) + " + " + std::to_string(z);
		}
};

class MediumMathPuzzle : public MathPuzzle {
	private:
		int x, y, z;
		RandomNumberGenerator& rd_;

	public:
		MediumMathPuzzle(RandomNumberGenerator& rd) : rd_(rd) {
			x = rd_.generateRandomNumber(1, 100);
			y = rd_.generateRandomNumber(1, 10);
			z = rd_.generateRandomNumber(1, 100);
			correctAnswer = x + y * z;
		}

		std::string toString() const override {
			return std::to_string(x) + " + " + std::to_string(y) + " * " + std::to_string(z);
		}
};

class HardMathPuzzle : public MathPuzzle {
	private:
		int x, y, z;
		RandomNumberGenerator& rd_;

	public:
		HardMathPuzzle(RandomNumberGenerator& rd) : rd_(rd) {
			x = rd_.generateRandomNumber(1, 15);
			y = rd_.generateRandomNumber(1, 100);
			z = rd_.generateRandomNumber(1, 100);
			correctAnswer = std::pow(x, 2) - y + z;
		}

		std::string toString() const override {
			return std::to_string(x) + "^2 - " + std::to_string(y) + " + " + std::to_string(z);
		}
};
