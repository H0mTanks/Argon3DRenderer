#include "App.hpp"


int main(int argc, char* args[]) {
	App* app = new App();

	while (app->is_running) {
		app->process_input();
		app->update();
		app->render();
	}

	app->destroy();

	return 0;
}