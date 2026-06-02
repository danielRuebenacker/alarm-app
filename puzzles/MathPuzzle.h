#include "../interfaces/IPuzzle.h"

class MathPuzzle : public IPuzzle {
	public:
		void display() override {}

		bool verifySolution() override { 
			return true;
		}
};
