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