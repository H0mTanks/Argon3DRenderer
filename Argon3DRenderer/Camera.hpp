#pragma once
#include "Vector.hpp"

struct Camera
{
	Vector3 position = { 0, 0, 0 };
	Vector3 direction = { 0, 0, 1 };
	Vector3 forward_velocity = { 0, 0, 0 };
	float yaw = 0.0f;
};

