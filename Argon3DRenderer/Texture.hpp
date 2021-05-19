#pragma once
#include "Color.hpp"
#include "upng.h"

class Texture2
{
public:
	float u = 0;
	float v = 0;

public:
    Texture2();
    Texture2(float u, float v);
};

//extern int texture_width;
//extern int texture_height;
//
//extern upng_t* png_texture;
//extern Color* mesh_texture;
//
//void load_png_texture_data(char const* filename);