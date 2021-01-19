#pragma once

#include "Macros.hpp"
#include "Types.hpp"

#include <random>

namespace Engine3D
{
	namespace Random
	{
		/**
		 * generate random number with uniform distribution U(min, max)
		 */
		double uniform(double min = 0.0, double max = 1.0);
		/**
		* generate random number with binomial distribution Bi(n, probability)
		*/
		u32 binomial(u32 n, double probability);
		/**
		* generate random number with exponential distribution Exp(1 / mean)
		*/
		double exponential(double mean);
		/**
		* generate random number with normal distribution N(mean, dispersion)
		*/
		double normal(double mean, double dispersion);
	}
};