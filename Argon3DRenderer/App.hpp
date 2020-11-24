#include <SDL.h>
#include <cstdint>

#pragma once

class App
{
private:
	SDL_Window* window{ nullptr };

public:
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	SDL_Renderer* renderer{ nullptr };
	SDL_Texture* display_buffer_texture{ nullptr };
	uint32_t* display_buffer{ nullptr };
	bool is_running{ false };

public:
	App();
	void process_input();
	void update();
	void render();
	void draw_grid();
	void draw_rect(int x, int y, int width, int height, uint32_t color);
	void render_display_buffer();
	void clear_display_buffer();
	void setup_display();
	void destroy();
};

