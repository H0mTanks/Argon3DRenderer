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
Color* mesh_texture = nullptr;