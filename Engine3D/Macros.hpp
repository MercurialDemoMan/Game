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

/**
 * define platform based on the compiler
 */

#define WINDOWS 0
#define LINUX   1
#define UNKNOWN 2

#if defined _MSC_VER
#define ENGINE3D_PLATFORM WINDOWS
#elif defined __GNUC__
#define ENGINE3D_PLATFORM LINUX
#else
#define ENGINE3D_PLATFORM UNKNOWN
#endif

/**
 * define always inline macro
 */
#if ENGINE3D_PLATFORM == WINDOWS
#define ENGINE3D_INLINE __forceinline
#elif ENGINE3D_PLATFORM == LINUX
#define ENGINE3D_INLINE __attribute__((always_inline))
#else
#define ENGINE3D_INLINE inline
#endif

/**
 * define asio version 
 */
#if ENGINE3D_PLATFORM == WINDOWS
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE

/**
 * fake usage of certain resource to silence the compiler
 */
#define ENGINE3D_UNUSED(x) ((void)(x))

/**
 * engine 3D version number
 */
#define ENGINE3D_VERSION "1.0.0"

/**
 * enable and import math constants
 */
#define NOMINMAX
#define _USE_MATH_DEFINES
#include <math.h>

/**
 * angle conversions
 */
#define ENGINE3D_RADTODEG(x) static_cast<float>((x) * 180.0f / M_PI)
#define ENGINE3D_DEGTORAD(x) static_cast<float>((x) * M_PI / 180.0f)

/**
 * class attributes
 */
#define ENGINE3D_INHERIT_CONSTRUCTORS(Class, Constructor) using Class::Constructor
#define ENGINE3D_NONCOPYABLE(Class) Class(Class const&) = delete; Class(Class const&&) = delete
#define ENGINE3D_NONMOVABLE(Class) Class& operator=(Class const&) = delete; Class& operator=(Class const&&) = delete