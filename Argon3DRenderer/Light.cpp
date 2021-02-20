#include "PrecompiledHeaders.hpp"
#include "Light.hpp"


Light::Light()
{
}

Light::Light(float x, float y, float z)
{
    direction = { x, y, z };
    direction = direction.normalize();
}

Light::Light(Vector3 const& v)
{
    direction = v.normalize();
}

Color Light::intensity(Color color, float factor)
{
    factor += 1;
    factor /= 2;

    //std::cout << factor << '\n';

    Color r = (color & 0x000000FF) * factor;
    Color g = (color & 0x0000FF00) * factor;
    Color b = (color & 0x00FF0000) * factor;
    Color a = (color & 0xFF000000);

    Color new_color = (r & 0x000000FF) | (g & 0x0000FF00) | (b & 0x00FF0000) | a;

    return new_color;
}
