#include <iostream>
#include "App.hpp"

App::App() {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << "Error Initializing SDL" << '\n';
		return;
	}

	window = SDL_CreateWindow(
		"Argon",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_BORDERLESS);

	if (!window) {
		std::cerr << "Error creating SDL Window" << '\n';
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cerr << "Error creating SDL_Renderer" << '\n';
		return;
	}

	is_running = true;
	return;
}


void App::process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT: {
			is_running = false;
			break;
		}
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				is_running = false;
				break;
			}
	}
}


void App::update() {

}


void App::render() {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}