#pragma once
#include "Triangle.hpp"
#include "Color.hpp"


namespace Draw
{
	enum Cull_type
	{
		CULL_NONE,
		CULL_BACKFACE
	};

	enum Render_type
	{
		RENDER_WIREFRAME,
		RENDER_WIREFRAME_VERTEX,
		RENDER_FILL_TRIANGLE,
		RENDER_FILL_TRIANGLE_WIREFRAME
	};

	void grid();
	void pixel(const int x, const int y, const Color color);
	void line(int x0, int y0, int x1, int y1, Color color);
	void rectangle(const int x, const int y, const int width, const int height, const Color color);
	void triangle(Triangle2 const& triangle, Color color);
	void fill_triangle(Triangle2 const& triangle, Color color);
	void fill_triangle(int ax, int ay, int bx, int by, int cx, int cy, Color color);
	void fill_bottom_triangle(Vector2_int const& a, Vector2_int const& b, Vector2_int const& m, Color color);
	void fill_bottom_triangle(int ax, int ay, int bx, int by, int cx, int cy, Color color);
	void fill_top_triangle(Vector2_int const& a, Vector2_int const& b, Vector2_int const& m, Color color);
	void fill_top_triangle(int ax, int ay, int bx, int by, int cx, int cy, Color color);

};