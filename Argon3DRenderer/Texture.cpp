#include "PrecompiledHeaders.hpp"
#include "Texture.hpp"

Texture2::Texture2()
{
}

Texture2::Texture2(float u, float v) : u(u), v(v)
{
}

int texture_height = 64;
int texture_width = 64;

upng_t* png_texture = nullptr;
Color* mesh_texture = nullptr;

void load_png_texture_data(char const* filename)
{
	png_texture = upng_new_from_file(filename);
	if (png_texture) {
		upng_decode(png_texture);
		if (upng_get_error(png_texture) == UPNG_EOK)
		{
			mesh_texture = (Color*)(upng_get_buffer(png_texture));
			texture_width = upng_get_width(png_texture);
			texture_height = upng_get_height(png_texture);
		}
	}
}
