#include <iostream>
#include <vector>
#include "App.hpp"
#include "Draw.hpp"
#include "Vector.hpp"
#include "Mesh.hpp"


Vector3 camera_position = Vector3(0, 0, -5);
Vector3 cube_rotation = Vector3(0, 0, 0);
Triangle triangles_to_render[N_MESH_FACES];


int App::WINDOW_WIDTH = 800;
int App::WINDOW_HEIGHT = 600;

SDL_Renderer* App::renderer = nullptr;
SDL_Texture* App::display_buffer_texture = nullptr;
uint32_t* App::display_buffer = nullptr;



// Initializes SDL, Window, Renderer and DisplayBuffer
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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::cerr << "Error creating SDL_Renderer" << '\n';
		return;
	}

	setup_display();

	is_running = true;
	return;
}



// Processes User Input and Events
void App::process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT: {
			is_running = false;
			break;
		}
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				is_running = false;
				break;
			}
		}
	}
}



// Updates each frame
void App::update() {

	cube_rotation.x += 0.01f;
	cube_rotation.y += 0.01f;
	cube_rotation.z += 0.01f;

	Triangle projected_triangle;

	for (int i = 0; i < N_MESH_FACES; i++) {
		Face mesh_face = mesh_faces[i];

		Vector3 face_vertices[3];
		face_vertices[0] = mesh_vertices[mesh_face.a - 1];
		face_vertices[1] = mesh_vertices[mesh_face.b - 1];
		face_vertices[2] = mesh_vertices[mesh_face.c - 1];

		for (int j = 0; j < 3; j++) {
			Vector3 transformed_vertex = face_vertices[j];
			transformed_vertex = transformed_vertex.rotate_xyz(cube_rotation.x, cube_rotation.y, cube_rotation.z);

			Vector2 projected_point = transformed_vertex.orthographic_project();

			projected_point.x += WINDOW_WIDTH / 2;
			projected_point.y += WINDOW_HEIGHT / 2;
			projected_triangle.points[j] = projected_point;
		}
		triangles_to_render[i] = projected_triangle;
	}
}


// Renders the current display buffer
void App::render() {
	Draw::grid();

	for (int i = 0; i < N_MESH_FACES; i++) {
		Triangle triangle = triangles_to_render[i];
		Draw::triangle(triangle, 0xFFFFFF00);
	}

	render_display_buffer();

	clear_display_buffer();

	SDL_RenderPresent(renderer);
}



//	Sets up the display buffer and should be called during Initialisation
void App::setup_display() {

	display_buffer = static_cast<uint32_t*> (new uint32_t[WINDOW_WIDTH * WINDOW_HEIGHT]);
	memset(display_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));

	if (!display_buffer) {
		std::cout << "Could not allocate memory for buffer";
	}

	display_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

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