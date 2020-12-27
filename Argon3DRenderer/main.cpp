#include "PrecompiledHeaders.hpp"
#include "App.hpp"
#include "Profiler.hpp"


int main(int argc, char* args[])
{
	Instrumentor::Get().BeginSession("Main");
	App* app = new App();
	while (app->is_running) {
		app->process_input();
		app->update();
		app->render();
	}

	app->destroy();
	Instrumentor::Get().EndSession();

	return 0;
}