#pragma once
#include "App.hpp"
#include "Triangle.hpp"

namespace Draw
{

	void grid();
	void pixel(const int x, const int y, const uint32_t color);
	void line(int x0, int y0, int x1, int y1, uint32_t color);
	void rectangle(const int x, const int y, const int width, const int height, const uint32_t color);
	void triangle(Triangle2D const& triangle, uint32_t color);
	void filled_triangle(Triangle2D const& triangle, uint32_t color);
	void filled_triangle(int ax, int ay, int bx, int by, int cx, int cy, uint32_t color);
	void fill_flat_bottom_triangle(Vector2_int const& a, Vector2_int const& b, Vector2_int const& m, uint32_t color);
	void fill_flat_bottom_triangle(int ax, int ay, int bx, int by, int cx, int cy, uint32_t color);
	void fill_flat_top_triangle(Vector2_int const& a, Vector2_int const& b, Vector2_int const& m, uint32_t color);
	void fill_flat_top_triangle(int ax, int ay, int bx, int by, int cx, int cy, uint32_t color);

};