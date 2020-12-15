#pragma once
#include "Vector.hpp"


class Face {
public:
	int a, b, c;
};

class Triangle {
public:
	Vector2 points[3];
};