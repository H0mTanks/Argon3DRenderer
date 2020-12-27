#pragma once
#include "Draw.hpp"

class App
{
private:
	SDL_Window* window{ nullptr };

public:
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	static SDL_Renderer* renderer;
	static SDL_Texture* display_buffer_texture;
	static Color* display_buffer;
	Draw::Render_type render_type = Draw::Render_type::RENDER_FILL_TRIANGLE_WIREFRAME;
	Draw::Cull_type cull_type = Draw::Cull_type::CULL_BACKFACE;
	bool is_running = false;

public:
	App();
	void process_input();
	void update();
	void render();
	void render_display_buffer();
	void clear_display_buffer();
	void setup_display();
	void quit();
	void destroy();
};

