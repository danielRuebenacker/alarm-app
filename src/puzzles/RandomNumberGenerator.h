#include <random>

class RandomNumberGenerator {
	private:
		std::random_device rd;
		std::mt19937 gen;
	public:
		RandomNumberGenerator() : gen(rd()) { }

		int generateRandomNumber(int min, int max) {
			std::uniform_int_distribution<int> distrib(min, max);
			return distrib(gen);
		}
};
