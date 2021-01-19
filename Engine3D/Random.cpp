#include "Random.hpp"

namespace Engine3D
{
	namespace Random
	{
		static std::random_device m_device {};
		static std::mt19937       m_mersenne_twister = std::mt19937(m_device());

		double uniform(double min, double max)
		{
			if (min > max)
			{
				std::swap(min, max);
			}

			std::uniform_real_distribution<double> distribution(min, max);
			return distribution(m_mersenne_twister);
		}
		u32 binomial(u32 n, double probability)
		{
			std::binomial_distribution<u32> distribution(n, probability);
			return distribution(m_mersenne_twister);
		}
		double exponential(double mean)
		{
			std::exponential_distribution<double> distribution(1.0 / mean);
			return distribution(m_mersenne_twister);
		}
		double normal(double mean, double dispersion)
		{
			std::normal_distribution<double> distribution(mean, dispersion);
			return distribution(m_mersenne_twister);
		}
	};
};