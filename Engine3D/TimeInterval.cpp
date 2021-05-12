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
#include "TimeInterval.hpp"

namespace Engine3D
{
	void TimeInterval::start()
	{
		m_start = std::chrono::system_clock::now();
	}

	float TimeInterval::end()
	{
		m_end = std::chrono::system_clock::now();

		std::chrono::duration<float> delta = m_end - m_start;

		return delta.count();
	}
};