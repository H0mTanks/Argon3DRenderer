#include <iostream>
#include "Draw.hpp"


void Draw::rectangle(const int x, const int y, const int width, const int height, const uint32_t color) {
	for (int j = y; j < y + height; j++) {
		for (int i = x; i < x + width; i++) {
			pixel(i, j, color);
		}
	}
}


void Draw::triangle(Triangle const& triangle, uint32_t color) {
	const int num_sides = 3;

	for (int i = 0; i < num_sides; i++) {
		//uncomment rectangle line to draw vertices explicitly
		/*rectangle(triangle.points[i].x, triangle.points[i].y, 4, 4, color);*/
		line(triangle.points[i].x, triangle.points[i].y, triangle.points[(i + 1) % num_sides].x, triangle.points[(i + 1) % num_sides].y, color);
	}
}

void Draw::triangle(const int x0, const int y0, const int x1, const int y1, const int x2, const int y2, uint32_t color) {
	const int num_sides = 3;

	for (int i = 0; i < num_sides; i++) {
		//uncomment rectangle lines to draw vertices explicitly
		/*rectangle(x0, y0, 4, 4, color);
		rectangle(x1, y1, 4, 4, color);
		rectangle(x2, y2, 4, 4, color);*/

		line(x0, y0, x1, y1, color);
		line(x1, y1, x2, y2, color);
		line(x2, y2, x0, y0, color);
	}
}


void Draw::line(const int x0, const int y0, const int x1, const int y1, uint32_t color) {
	int delta_x = x1 - x0;
	int delta_y = y1 - y0;

	int longest_side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)longest_side_length;
	float y_inc = delta_y / (float)longest_side_length;

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= longest_side_length; i++) {
		pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
}



void Draw::pixel(const int x, const int y, const uint32_t color) {
	if (x < App::WINDOW_WIDTH && y < App::WINDOW_HEIGHT && x >= 0 && y >= 0) {
		App::display_buffer[App::WINDOW_WIDTH * y + x] = color;
	}
}

//Draws a dark grey grid of 30 pixel boxes
void Draw::grid() {
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