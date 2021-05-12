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
#pragma once

#include "Macros.hpp"
#include "Types.hpp"

namespace Engine3D
{
	namespace Random
	{
		/**
		 * generate random number with uniform distribution U(min, max)
		 */
		float uniform(float min = 0, float max = 1);
		/**
		* generate random number with binomial distribution Bi(n, probability)
		*/
		u32 binomial(u32 n, float probability);
		/**
		* generate random number with exponential distribution Exp(1 / mean)
		*/
		float exponential(float mean);
		/**
		* generate random number with normal distribution N(mean, dispersion)
		*/
		float normal(float mean, float dispersion);
		
		float perlin1D(float x);
	}
};