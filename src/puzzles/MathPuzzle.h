#pragma once
#include "../interfaces/IPuzzle.h"

class MathPuzzle : public IPuzzle {
	public:
		void display(UIWidget* wrapper) override {}

		bool verifySolution() override { 
			return true;
		}
};
