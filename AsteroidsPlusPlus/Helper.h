#pragma once

#define CONSTANT_PI 3.14159265f

#include "glm/glm.hpp"
#include <cmath>
#include <algorithm>

inline float Deg2Rad(float angle)
{
	return angle * CONSTANT_PI / 180.0f;
}

inline glm::vec2 Rotate2D(glm::vec2 v, float angle)
{
	glm::mat2 rotation_matrix = { { cosf(Deg2Rad(angle)), -sinf(Deg2Rad(angle)) }, { sinf(Deg2Rad(angle)), cosf(Deg2Rad(angle)) } };
	return rotation_matrix * v;
}

inline float Clamp(float n, float lower, float upper) 
{
	return std::max(lower, std::min(n, upper));
}

inline float Lerp(float a, float b, float t)
{
	if(a < b)
		return Clamp(a + t * (b - a), a, b);
	else
		return Clamp(a + t * (b - a), b, a);
}

inline float Decay(float current, float rate)
{
	float sign = fabs(current) / current;
	return sign * Lerp(fabs(current), 0.0f, rate);
}

inline float RandNormalized()
{
	return (float)rand() / (float)RAND_MAX;
}

inline float RandRange(float min, float max)
{
	return min + (RandNormalized()) * (max - min);
}

inline glm::vec2 RandVec2(float magnitude)
{
	return magnitude * glm::normalize(glm::vec2(RandRange(-1.0f, 1.0f), RandRange(-1.0f, 1.0f)));
}