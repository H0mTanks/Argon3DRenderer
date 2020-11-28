#pragma once

struct Vector2 {
	float x;
	float y;
};


struct Vector3 {
	float x;
	float y;
	float z;
	Vector2 project() {
		float fov_factor = 129;
		Vector3& original_vector = *this;
		Vector2 projection_vector = { fov_factor * original_vector.x, fov_factor * original_vector.y };
		return projection_vector;
	}
};