#include <iostream>
#include "App.hpp"

int App::WINDOW_WIDTH = 800;
int App::WINDOW_HEIGHT = 600;


App::App() {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << "Error Initializing SDL" << '\n';
		return;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	WINDOW_WIDTH = display_mode.w;
	WINDOW_HEIGHT = display_mode.h;

	window = SDL_CreateWindow(
		"Argon",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
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

	draw_grid();
	draw_rect(100, 100, 100, 100, 0xFFFFFFFF);

	render_display_buffer();

	clear_display_buffer();

	SDL_RenderPresent(renderer);
}


void App::setup_display() {
	display_buffer = static_cast<uint32_t*> (new uint32_t[WINDOW_WIDTH * WINDOW_HEIGHT]);
	memset(display_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));

	if (!display_buffer) {
		std::cout << "Could not allocate memory for buffer";
	}

	display_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
}


void App::clear_display_buffer() {
	/*for (int y = 0; y < WINDOW_HEIGHT; y++) {
		for (int x = 0; x < WINDOW_WIDTH; x++) {
			display_buffer[WINDOW_WIDTH * y + x] = color;
		}
	}*/

	memset(display_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));
}


void App::draw_rect(int x, int y, int width, int height, uint32_t color) {
	for (int j = y; j < y + height; j++) {
		for (int i = x; i < x + width; i++) {
			display_buffer[WINDOW_WIDTH * j + i] = color;
		}
	}
}


void App::destroy() {
	delete display_buffer;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void App::render_display_buffer() {
	SDL_UpdateTexture(display_buffer_texture, NULL, display_buffer, WINDOW_WIDTH * sizeof(uint32_t));
	SDL_RenderCopy(renderer, display_buffer_texture, NULL, NULL);
}


//draws a white grid of 10 pixel boxes
void App::draw_grid() {
	//vertical lines
	for (int y = 0; y < WINDOW_HEIGHT; y++) {
		for (int x = 0; x < WINDOW_WIDTH; x += 10) {
			display_buffer[WINDOW_WIDTH * y + x] = 0xFFFFFFFF;
		}
	}

	//horizontal lines
	for (int y = 0; y < WINDOW_HEIGHT; y += 10) {
		for (int x = 0; x < WINDOW_WIDTH; x++) {
			display_buffer[WINDOW_WIDTH * y + x] = 0xFFFFFFFF;
		}
	}
}
