#pragma once
#include "App.hpp"

class Draw {
public:
	static void draw_grid();
	static void draw_rect(int x, int y, int width, int height, uint32_t color);
};

