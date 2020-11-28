#include <iostream>
#include <vector>
#include "App.hpp"
#include "Draw.hpp"
#include "Vector.hpp"


static const int N_VECTORS = 9 * 9 * 9;
std::vector<Vector3> cube_vectors;
std::vector<Vector2> projection_vectors;

int App::WINDOW_WIDTH = 800;
int App::WINDOW_HEIGHT = 600;
SDL_Renderer* App::renderer = nullptr;
SDL_Texture* App::display_buffer_texture = nullptr;
uint32_t* App::display_buffer = nullptr;


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

	setup_display();

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
	for (int i = 0; i < N_VECTORS; i++) {
		Vector3 vector = cube_vectors[i];
		Vector2 projection_vector = vector.project();
		projection_vectors.push_back(projection_vector);
	}
}


void App::render() {
	/*Draw::draw_grid();*/
	for (int i = 0; i < N_VECTORS; i++) {
		Draw::draw_rect(static_cast<int>(projection_vectors[i].x + WINDOW_WIDTH/2), 
			static_cast<int>(projection_vectors[i].y + WINDOW_HEIGHT/2), 4, 4, 0xFFFF00FF);
	}

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

	cube_vectors.reserve(N_VECTORS);
	projection_vectors.reserve(N_VECTORS);

	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				Vector3 new_vector = { x, y, z };
				cube_vectors.push_back(new_vector);
			} 
		}
	}

}


void App::clear_display_buffer() {
	memset(display_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));
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

