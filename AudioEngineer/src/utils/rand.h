#pragma once

#include <ctime>
#include <random>
#include "..\defines.h"

namespace audioengineer {
	namespace utils {

		extern std::default_random_engine generator;

		void rand_init();
		int randInt(int min, int max);
		uint randUint(uint min, uint max);
		float rand01();
		float randFloat(float min, float max);
		float randNormal(float mu, float sigma);

	}
}