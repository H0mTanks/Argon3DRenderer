#include <SDL.h>

#pragma once

class App
{
private:
	SDL_Window* window{ nullptr };

public:
	SDL_Renderer* renderer{ nullptr };
	bool is_running{ true };

public:
	App();
	void process_input();
	void update();
	void render();
};

