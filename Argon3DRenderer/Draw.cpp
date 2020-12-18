#include <iostream>
#include "Draw.hpp"


void Draw::rectangle(const int x, const int y, const int width, const int height, const uint32_t color)
{
	for (int j = y; j < y + height; j++) {
		for (int i = x; i < x + width; i++) {
			pixel(i, j, color);
		}
	}
}


void Draw::triangle(Triangle2D const& triangle, uint32_t color)
{
	line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, color);
	line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, color);
	line(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, color);
}

void Draw::filled_triangle(Triangle2D const& triangle, uint32_t color)
{
	Vector2 a = triangle.points[0];
	Vector2 b = triangle.points[1];
	Vector2 c = triangle.points[2];
	
	if (a.y > b.y) {
		std::swap(a.y, b.y);
		std::swap(a.x, b.x);
	}

	if (b.y > c.y) {
		std::swap(b.y, c.y);
		std::swap(b.x, c.x);
	}

	if (a.y > b.y) {
		std::swap(a.y, b.y);
		std::swap(a.x, b.x);
	}

	Vector2 m((static_cast<float>((c.x - a.x) * (b.y - a.y))) / static_cast<float>(c.y - a.y) + a.x, a.y);

	fill_flat_bottom_triangle(a, b, m, color);

	fill_flat_top_triangle(a, m, c, color);

}

void Draw::fill_flat_bottom_triangle(Vector2 const& a, Vector2 const& b, Vector2 const& m, uint32_t color)
{
	float slope_ab = (a.x - b.x) / (a.y - b.y);
	float slope_am = (a.x - m.x) / (a.y - m.y);

	float x_start = a.x;
	float x_end = a.x;

	for (int y = a.y; y <= b.y; y++) {
		line(x_start, y, x_end, y, color);
		x_start += slope_ab;
		x_end += slope_am;
	}
}

void Draw::fill_flat_top_triangle(Vector2 const& a, Vector2 const& b, Vector2 const& m, uint32_t color)
{

}


void Draw::line(int x0, int y0, int x1, int y1, uint32_t color)
{
	bool steep = false;
	int delta_x = x0 - x1;
	int delta_y = y0 - y1;
	if (std::abs(delta_x) < std::abs(delta_y)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1 - t) + y1 * t;
		if (steep) {
			pixel(y, x, color);
		}
		else {
			pixel(x, y, color);
		}
	}
}



void Draw::pixel(const int x, const int y, const uint32_t color)
{
	if (x < App::WINDOW_WIDTH && y < App::WINDOW_HEIGHT && x >= 0 && y >= 0) {
		App::display_buffer[App::WINDOW_WIDTH * y + x] = color;
	}
}

//Draws a dark grey grid of 30 pixel boxes
void Draw::grid()
{
	//vertical lines
	for (int y = 0; y < App::WINDOW_HEIGHT; y++) {
		for (int x = 0; x < App::WINDOW_WIDTH; x += 30) {
			App::display_buffer[App::WINDOW_WIDTH * y + x] = 0x101010FF;
		}
	}

	//horizontal lines
	for (int y = 0; y < App::WINDOW_HEIGHT; y += 30) {
		for (int x = 0; x < App::WINDOW_WIDTH; x++) {
			App::display_buffer[App::WINDOW_WIDTH * y + x] = 0x101010FF;
		}
	}
}