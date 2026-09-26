#pragma once
#include <cstdint>
#include <random>

class RandomNumberGenerator {
	private:
	std::random_device rd;
	std::mt19937 gen;
	public:
	RandomNumberGenerator() : gen(rd()) { }

	// fixed seed, so puzzle generation can be made deterministic in tests
	explicit RandomNumberGenerator(std::uint32_t seed) : gen(seed) { }

	int generateRandomNumber(int min, int max) {
		std::uniform_int_distribution<int> distrib(min, max);
		return distrib(gen);
	}
};
