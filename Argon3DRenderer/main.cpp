#include "App.hpp"
#include "main.h"


int main(int argc, char* args[]) {
	App* app = new App();
	app->setup_display();

	while (app->is_running) {
		app->process_input();
		app->update();
		app->render();
	}

	app->destroy();

	return 0;
}