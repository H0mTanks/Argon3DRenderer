#pragma once
#include <SDL.h>
#include <cstdint>

class App
{
private:
	SDL_Window* window{ nullptr };

public:
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	static SDL_Renderer* renderer;
	static SDL_Texture* display_buffer_texture;
	static uint32_t* display_buffer;
	bool is_running{ false };

public:
	App();
	void process_input();
	void update();
	void render();
	void render_display_buffer();
	void clear_display_buffer();
	void setup_display();
	void destroy();
};

