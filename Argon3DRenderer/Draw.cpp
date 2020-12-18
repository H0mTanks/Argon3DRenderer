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
	Vector2_int a(triangle.points[0].x, triangle.points[0].y);
	Vector2_int b(triangle.points[1].x, triangle.points[1].y);
	Vector2_int c(triangle.points[2].x, triangle.points[2].y);

	if (a.y == b.y && a.y == c.y) { //protect against degenerate triangles to avoid divisions by zero later
		return;
	}

	if (a.y > b.y) {
		std::swap(a, b);
	}

	if (b.y > c.y) {
		std::swap(b, c);
	}

	if (a.y > b.y) {
		std::swap(a, b);
	}


	Vector2_int m(((c.x - a.x) * (b.y - a.y)) / (c.y - a.y) + a.x, b.y);

	fill_flat_bottom_triangle(a, b, m, color);
	
	fill_flat_top_triangle(b, m, c, color);

}


void Draw::filled_triangle(int ax, int ay, int bx, int by, int cx, int cy, uint32_t color)
{
	if (ay == by && ay == cy) { //protect against degenerate triangles to avoid divisions by zero later
		return;
	}

	if (ay > by) {
		std::swap(ay, by);
		std::swap(ax, bx);
	}

	if (by > cy) {
		std::swap(by, cy);
		std::swap(bx, cx);
	}

	if (ay > by) {
		std::swap(ay, by);
		std::swap(ax, bx);
	}

	int mx = ((float)(cx - ax) * (by - ay)) / (float)(cy - ay) + ax;
	int my = by;


	fill_flat_bottom_triangle(ax, ay, bx, by, mx, my, color);

	fill_flat_top_triangle(bx, by, mx, my, cx, cy, color);
}


void Draw::fill_flat_bottom_triangle(Vector2_int const& a, Vector2_int const& b, Vector2_int const& m, uint32_t color)
{
	
	float slope_ab = (float)(a.x - b.x) / (a.y - b.y);
	float slope_am = (float)(a.x - m.x) / (a.y - m.y);

	float x_start = a.x;
	float x_end = a.x;

	
	for (int y = a.y; y <= b.y; y++) {

		line(x_start, y, x_end, y, color);

		x_start += slope_ab;
		x_end += slope_am;
	}
}

void Draw::fill_flat_bottom_triangle(int ax, int ay, int bx, int by, int mx, int my, uint32_t color)
{
	float slope_ab = (float)(ax - bx) / (ay - by);
	float slope_am = (float)(ax - mx) / (ay - my);

	float x_start = ax;
	float x_end = ax;


	for (int y = ay; y <= by; y++) {

		line(x_start, y, x_end, y, color);

		x_start += slope_ab;
		x_end += slope_am;
	}
}


void Draw::fill_flat_top_triangle(Vector2_int const& b, Vector2_int const& m, Vector2_int const& c, uint32_t color)
{
	float slope_cb = (float)(c.x - b.x) / (c.y - b.y);
	float slope_cm = (float)(c.x - m.x) / (c.y - m.y);

	float x_start = c.x;
	float x_end = c.x;

	
	for (int y = c.y; y >= b.y; y--) {

		line(x_start, y, x_end, y, color);

		x_start -= slope_cb;
		x_end -= slope_cm;
	}
}

void Draw::fill_flat_top_triangle(int bx, int by, int mx, int my, int cx, int cy, uint32_t color)
{
	float slope_cb = (float)(cx - bx) / (cy - by);
	float slope_cm = (float)(cx - mx) / (cy - my);

	float x_start = cx;
	float x_end = cx;


	for (int y = cy; y >= by; y--) {

		line(x_start, y, x_end, y, color);

		x_start -= slope_cb;
		x_end -= slope_cm;
	}
}


void Draw::line(int x0, int y0, int x1, int y1, uint32_t color)
{

	int delta_x(x1 - x0);
	
	signed char const ix((delta_x > 0) - (delta_x < 0));

	delta_x = std::abs(delta_x) << 1;

	int delta_y(y1 - y0);
	
	signed char const iy((delta_y > 0) - (delta_y < 0));
	delta_y = std::abs(delta_y) << 1;

	pixel(x0, y0, color);

	if (delta_x >= delta_y) {
		int error(delta_y - (delta_x >> 1));

		while (x0 != x1) {
			if ((error > 0) || (!error && (ix > 0))) {
				error -= delta_x;
				y0 += iy;
			}

			error += delta_y;
			x0 += ix;

			pixel(x0, y0, color);
		}
	}
	else {
		int error(delta_x - (delta_y >> 1));

		while (y0 != y1) {
			if ((error > 0) || (!error && (iy > 0))) {
				error -= delta_y;
				x0 += ix;
			}

			error += delta_x;
			y0 += iy;

			pixel(x0, y0, color);
		}
	}

	//DDA Algorithm
	/*int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)longest_side_length;
	float y_inc = delta_y / (float)longest_side_length;

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= longest_side_length; i++) {
		pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}*/
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