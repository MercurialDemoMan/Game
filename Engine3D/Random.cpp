/*
This file is part of Engine3D.

Engine3D is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Engine3D is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Engine3D.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "Random.hpp"

#include <random>

namespace Engine3D
{
	namespace Random
	{
		std::random_device m_random_device {};
		std::mt19937       m_mersenne_twister = std::mt19937(m_random_device());

		float uniform(float min, float max)
		{
			if (min > max)
			{
				std::swap(min, max);
			}

			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(m_mersenne_twister);
		}
		u32 binomial(u32 n, float probability)
		{
			std::binomial_distribution<u32> distribution(n, probability);
			return distribution(m_mersenne_twister);
		}
		float exponential(float mean)
		{
			std::exponential_distribution<float> distribution(1.0 / mean);
			return distribution(m_mersenne_twister);
		}
		float normal(float mean, float dispersion)
		{
			std::normal_distribution<float> distribution(mean, dispersion);
			return distribution(m_mersenne_twister);
		}

		float perlin1D(float x)
		{
			static constexpr const u8 hash[256] =
			{
				225, 39, 141, 195, 96, 128, 45, 144, 155,
				243, 255, 210, 5, 57, 227, 147, 89,
				0, 132, 86, 161, 232, 161, 53, 85,
				18, 250, 80, 213, 14, 66, 199, 85,
				46, 73, 79, 193, 89, 138, 48, 20,
				175, 26, 182, 214, 79, 8, 157, 105,
				60, 74, 18, 200, 144, 205, 139, 89,
				71, 109, 201, 206, 84, 93, 91, 222,
				121, 240, 125, 75, 77, 70, 46, 29,
				199, 228, 28, 161, 150, 98, 166, 71,
				214, 110, 7, 248, 38, 254, 225, 6,
				248, 87, 87, 129, 251, 191, 37, 12,
				254, 219, 124, 144, 31, 55, 169, 242,
				148, 233, 200, 229, 204, 253, 140, 175,
				192, 253, 23, 37, 176, 232, 168, 253,
				73, 243, 91, 86, 93, 45, 36, 4,
				86, 39, 239, 127, 166, 192, 73, 79,
				125, 25, 248, 125, 83, 24, 64, 104,
				110, 180, 7, 124, 8, 100, 142, 27,
				162, 171, 46, 181, 218, 20, 151, 124,
				0, 218, 53, 95, 181, 75, 199, 49,
				209, 70, 81, 28, 165, 125, 84, 63,
				139, 11, 197, 241, 87, 242, 36, 171,
				226, 233, 133, 143, 93, 173, 148, 34,
				52, 157, 213, 189, 107, 89, 112, 9,
				1, 232, 220, 103, 3, 208, 48, 121,
				194, 46, 176, 14, 209, 133, 113, 32,
				21, 9, 20, 85, 213, 62, 169, 236,
				132, 104, 228, 23, 96, 55, 157, 26,
				177, 30, 133, 123, 131, 68, 185, 255,
				65, 198, 64, 69, 29, 96, 124, 49,
				74, 89, 69, 14, 33, 116, 138,
			};

			auto get_hash = [&](s32 i) -> u8
			{
				return hash[i & 0xFF];
			};

			auto grad = [&](s32 hash, float x)
			{
				s32 h = hash & 0x0F;
				float grad = 1.0f + (h & 0b111);
				if ((h & 8) != 0)
				{
					grad = -grad;
				}
				return (grad * x);
			};

			s32 a0 = s32(x);
			s32 a1 = a0 + 1;

			float x0 = x - a0;
			float x1 = x0 - 1;

			float t0 = 1 - x0 * x0;
			t0 *= t0;
			float n0 = t0 * t0 * grad(get_hash(a0), x0);

			float t1 = 1 - x1 * x1;
			t1 *= t1;
			float n1 = t1 * t1 * grad(get_hash(a1), x1);

			return 0.395f * (n0 + n1);
		}
	}
};