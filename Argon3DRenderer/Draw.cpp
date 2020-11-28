#include "Draw.hpp"
#include <iostream>

void Draw::draw_rect(int x, int y, int width, int height, uint32_t color) {
	for (int j = y; j < y + height; j++) {
		for (int i = x; i < x + width; i++) {
			draw_pixel(i, j, color);
		}
	}
}

void Draw::draw_pixel(int x, int y, uint32_t color) {
	if (x < App::WINDOW_WIDTH && y < App::WINDOW_HEIGHT && x >= 0 && y >= 0) {
		App::display_buffer[App::WINDOW_WIDTH * y + x] = color;
	}
}

//draws a white grid of 10 pixel boxes
void Draw::draw_grid() {
	//vertical lines
	for (int y = 0; y < App::WINDOW_HEIGHT; y++) {
		for (int x = 0; x < App::WINDOW_WIDTH; x += 10) {
			App::display_buffer[App::WINDOW_WIDTH * y + x] = 0xFFFFFFFF;
		}
	}

	//horizontal lines
	for (int y = 0; y < App::WINDOW_HEIGHT; y += 10) {
		for (int x = 0; x < App::WINDOW_WIDTH; x++) {
			App::display_buffer[App::WINDOW_WIDTH * y + x] = 0xFFFFFFFF;
		}
	}
}