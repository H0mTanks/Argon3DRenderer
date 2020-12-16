#pragma once
#include "App.hpp"
#include "Triangle.hpp"

namespace Draw {

	void grid();
	void pixel(const int x, const int y, const uint32_t color);
	void line(const int x0, const int y0, const int x1, const int y1, uint32_t color);
	void rectangle(const int x, const int y, const int width, const int height, const uint32_t color);
	void triangle(Triangle const& triangle, uint32_t color);
	void triangle(const int x0, const int y0, const int x1, const int y1, const int x2, const int y2, uint32_t color);
};

