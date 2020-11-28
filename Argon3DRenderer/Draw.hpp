#pragma once
#include "App.hpp"

class Draw {
public:
	static void draw_grid();
	static void draw_rect(int x, int y, int width, int height, uint32_t color);
	static void draw_pixel(int x, int y, uint32_t color);
	
};

