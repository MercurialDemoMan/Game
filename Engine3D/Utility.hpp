#pragma once

#include <functional>
#include <cstdio>

#include "Macros.hpp"

namespace Engine3D
{
	/**
	 * try block
	 */
	template<typename T>
	ENGINE3D_INLINE void inline_try(std::function<void(void)> f, const char* message_prefix = nullptr)
	{
		try
		{
			std::invoke(f);
		}
		catch (T& e)
		{
			if (message_prefix)
			{
				std::printf("%s: %s\n", message_prefix, e.what());
			}
			else
			{
				std::printf("%s\n", e.what());
			}
		}
	}
};