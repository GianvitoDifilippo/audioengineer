#include "rand.h"

namespace audioengineer {
	namespace utils {

		std::default_random_engine generator;

		void rand_init()
		{
			srand((uint)time(0));
		}

		int randInt(int min, int max)
		{
			return rand() % (max - min) + min;
		}

		uint randUint(uint min, uint max)
		{
			return (uint)rand() % (max - min) + min;
		}

		float rand01()
		{
			return (float)rand() / (float)RAND_MAX;
		}

		float randFloat(float min, float max)
		{
			return rand01() * (max - min) + min;
		}

		float randNormal(float mu, float sigma)
		{
			return std::normal_distribution<float>(mu, sigma)(generator);
		}

	}
}