#pragma once
#include "App.hpp"

namespace Draw {

	void draw_grid();
	void draw_rect(const int x, const int y, const int width, const int height, const uint32_t color);
	void draw_pixel(const int x, const int y, const uint32_t color);
};

