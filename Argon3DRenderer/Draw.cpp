#include "PrecompiledHeaders.hpp"
#include "App.hpp"
#include "Draw.hpp"



void Draw::rectangle(const int x, const int y, const int width, const int height, const Color color)
{
	for (int j = y; j < y + height; j++) {
		for (int i = x; i < x + width; i++) {
			pixel(i, j, color);
		}
	}
}


void Draw::triangle(Triangle4 const& triangle, Color color)
{
	line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, color);
	line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, color);
	line(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, color);
}

void Draw::fill_triangle(Triangle4 const& triangle, Color color)
{
	Vector4_int a(triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w);
	Vector4_int b(triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w);
	Vector4_int c(triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w);

	if (a.y == b.y && a.y == c.y) { //protect against degenerate triangles to avoid divisions by zero later
		return;
	}

	if (a.y > b.y) {
		std::swap(a, b);
	}

	if (b.y > c.y) {
		std::swap(b, c);
	}

	if (a.y > b.y) {
		std::swap(a, b);
	}


	Vector2_int m(((c.x - a.x) * (b.y - a.y)) / (c.y - a.y) + a.x, b.y);

	fill_bottom_triangle(a, b, c, m, color);
	
	fill_top_triangle(a, b, c, m, color);

}


void Draw::fill_bottom_triangle(Vector4_int const& a, Vector4_int const& b, Vector4_int const& c, Vector2_int const& m, Color color)
{	
	float slope_ab = (float)(a.x - b.x) / (a.y - b.y);
	float slope_am = (float)(a.x - m.x) / (a.y - m.y);

	float x_start = a.x;
	float x_end = a.x;

	
	for (int y = a.y; y <= b.y; y++) {

		if (x_start <= x_end) {
			for (int x = x_start; x <= x_end; x++) {
				fill_pixel(x, y, color, a, b, c);
			}
		}
		else {
			for (int x = x_end; x <= x_start; x++) {
				fill_pixel(x, y, color, a, b, c);
			}
		}

		x_start += slope_ab;
		x_end += slope_am;
	}
}


void Draw::fill_top_triangle(Vector4_int const& a, Vector4_int const& b, Vector4_int const& c, Vector2_int const& m, Color color)
{
	float slope_cb = (float)(c.x - b.x) / (c.y - b.y);
	float slope_cm = (float)(c.x - m.x) / (c.y - m.y);

	float x_start = c.x;
	float x_end = c.x;

	
	for (int y = c.y; y >= b.y; y--) {

		if (x_start <= x_end) {
			for (int x = x_start; x <= x_end; x++) {
				fill_pixel(x, y, color, a, b, c);
			}
		}
		else {
			for (int x = x_end; x <= x_start; x++) {
				fill_pixel(x, y, color, a, b, c);
			}
		}

		x_start -= slope_cb;
		x_end -= slope_cm;
	}
}


void Draw::fill_triangle(int ax, int ay, int bx, int by, int cx, int cy, Color color)
{
	if (ay == by && ay == cy) { //protect against degenerate triangles to avoid divisions by zero later
		return;
	}

	if (ay > by) {
		std::swap(ay, by);
		std::swap(ax, bx);
	}

	if (by > cy) {
		std::swap(by, cy);
		std::swap(bx, cx);
	}

	if (ay > by) {
		std::swap(ay, by);
		std::swap(ax, bx);
	}

	int mx = ((float)(cx - ax) * (by - ay)) / (float)(cy - ay) + ax;
	int my = by;


	fill_bottom_triangle(ax, ay, bx, by, mx, my, color);

	fill_top_triangle(bx, by, mx, my, cx, cy, color);
}


void Draw::fill_bottom_triangle(int ax, int ay, int bx, int by, int mx, int my, Color color)
{
	float slope_ab = (float)(ax - bx) / (ay - by);
	float slope_am = (float)(ax - mx) / (ay - my);

	float x_start = ax;
	float x_end = ax;


	for (int y = ay; y <= by; y++) {

		line(x_start, y, x_end, y, color);

		x_start += slope_ab;
		x_end += slope_am;
	}
}


void Draw::fill_top_triangle(int bx, int by, int mx, int my, int cx, int cy, Color color)
{
	float slope_cb = (float)(cx - bx) / (cy - by);
	float slope_cm = (float)(cx - mx) / (cy - my);

	float x_start = cx;
	float x_end = cx;


	for (int y = cy; y >= by; y--) {

		line(x_start, y, x_end, y, color);

		x_start -= slope_cb;
		x_end -= slope_cm;
	}
}

void Draw::textured_triangle(Triangle4 const& triangle, Color* texture)
{
	Vector4_int a(triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w);
	Vector4_int b(triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w);
	Vector4_int c(triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w);
	Texture2 a_uv = triangle.tex_coords[0];
	Texture2 b_uv = triangle.tex_coords[1];
	Texture2 c_uv = triangle.tex_coords[2];


	if (a.y == b.y && a.y == c.y) { //protect against degenerate triangles to avoid divisions by zero later
		return;
	}
	if (a.y > b.y) {
		std::swap(a, b);
		std::swap(a_uv, b_uv);
	}

	if (b.y > c.y) {
		std::swap(b, c);
		std::swap(b_uv, c_uv);
	}

	if (a.y > b.y) {
		std::swap(a, b);
		std::swap(a_uv, b_uv);
	}

	//flip v because otherwise the texture gets mapped as flipped because the obj file doesn't specify coordinates
	//from the bottom left
	a_uv.v = 1 - a_uv.v;
	b_uv.v = 1 - b_uv.v;
	c_uv.v = 1 - c_uv.v;


	Vector2_int m(((c.x - a.x) * (b.y - a.y)) / (c.y - a.y) + a.x, b.y);

	textured_bottom_triangle(a, b, c, m, a_uv, b_uv, c_uv, texture);

	textured_top_triangle(a, b, c, m, a_uv, b_uv, c_uv, texture);

	/*float inv_slope_1 = 0;
	float inv_slope_2 = 0;

	inv_slope_1 = (float)(b.x - a.x) / abs(b.y - a.y);
	inv_slope_2 = (float)(c.x - a.x) / abs(c.y - a.y);

	for (int y = a.y; y <= b.y; y++) {
		int x_start = b.x + (y - b.y) * inv_slope_1;
		int x_end = a.x + (y - a.y) * inv_slope_2;

		if (x_end < x_start) {
			std::swap(x_end, x_start);
		}

		for (int x = x_start; x < x_end; x++) {
			pixel(x, y, 0xFF00FFFF);
		}
	}*/
}


void Draw::textured_bottom_triangle(Vector4_int const& a, Vector4_int const& b, Vector4_int const& c, Vector2_int const& m,
	Texture2 const& a_uv, Texture2 const& b_uv, Texture2 const& c_uv, Color* texture)
{
	float slope_ab = (float)(a.x - b.x) / (a.y - b.y);
	float slope_am = (float)(a.x - m.x) / (a.y - m.y);

	float x_start = a.x;
	float x_end = a.x;


	for (int y = a.y; y <= b.y; y++) {

		//line(x_start, y, x_end, y, color);

		/*int x_start = b.x + (y - b.y) * slope_ab;
		int x_end = a.x + (y - a.y) * slope_am;*/

		if (x_start <= x_end) {
			for (int x = x_start; x <= x_end; x++) {
				//pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFF00FFFF : 0x000000FF);
				texel(x, y, a, b, c, a_uv, b_uv, c_uv, texture);
			}
		}
		else {
			for (int x = x_end; x <= x_start; x++) {
				//pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFF00FFFF : 0x000000FF);
				texel(x, y, a, b, c, a_uv, b_uv, c_uv, texture);
			}
		}


		x_start += slope_ab;
		x_end += slope_am;
	}
}


void Draw::textured_top_triangle(Vector4_int const& a, Vector4_int const& b, Vector4_int const& c, Vector2_int const& m,
	Texture2 const& a_uv, Texture2 const& b_uv, Texture2 const& c_uv, Color* texture)
{
	float slope_cb = (float)(c.x - b.x) / (c.y - b.y);
	float slope_cm = (float)(c.x - m.x) / (c.y - m.y);

	float x_start = c.x;
	float x_end = c.x;


	for (int y = c.y; y >= b.y; y--) {

		if (x_start <= x_end) {
			for (int x = x_start; x <= x_end; x++) {
				//pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFF00FFFF : 0x000000FF);
				texel(x, y, a, b, c, a_uv, b_uv, c_uv, texture);
			}
		}
		else {
			for (int x = x_end; x <= x_start; x++) {
				//pixel(x, y, (x % 2 == 0 && y % 2 == 0) ? 0xFF00FFFF : 0x000000FF);
				texel(x, y, a, b, c, a_uv, b_uv, c_uv, texture);
			}
		}

		x_start -= slope_cb;
		x_end -= slope_cm;
	}
}


void Draw::line(int x0, int y0, int x1, int y1, Color color)
{

	int delta_x(x1 - x0);
	
	signed char const ix((delta_x > 0) - (delta_x < 0));

	delta_x = std::abs(delta_x) << 1;

	int delta_y(y1 - y0);
	
	signed char const iy((delta_y > 0) - (delta_y < 0));
	delta_y = std::abs(delta_y) << 1;

	pixel(x0, y0, color);

	if (delta_x >= delta_y) {
		int error(delta_y - (delta_x >> 1));

		while (x0 != x1) {
			if ((error > 0) || (!error && (ix > 0))) {
				error -= delta_x;
				y0 += iy;
			}

			error += delta_y;
			x0 += ix;

			pixel(x0, y0, color);
		}
	}
	else {
		int error(delta_x - (delta_y >> 1));

		while (y0 != y1) {
			if ((error > 0) || (!error && (iy > 0))) {
				error -= delta_y;
				x0 += ix;
			}

			error += delta_x;
			y0 += iy;

			pixel(x0, y0, color);
		}
	}

	//DDA Algorithm
	/*int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)longest_side_length;
	float y_inc = delta_y / (float)longest_side_length;

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= longest_side_length; i++) {
		pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}*/
}


void Draw::pixel(const int x, const int y, const Color color)
{
	if (x < App::WINDOW_WIDTH && y < App::WINDOW_HEIGHT && x >= 0 && y >= 0) {
		App::display_buffer[App::WINDOW_WIDTH * y + x] = color;
	}
}

void Draw::fill_pixel(const int x, const int y, const Color color, Vector4_int const& a, Vector4_int const& b, Vector4_int const& c)
{
	Vector2_int p(x, y);
	Vector3 weights = Vector3::barycentric_weights(a.to_vec2_int(), b.to_vec2_int(), c.to_vec2_int(), p);

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	float interpolated_reciprocal_w = (1 / a.w) * alpha + (1 / b.w) * beta + (1 / c.w) * gamma;

	if (interpolated_reciprocal_w > App::z_buffer[(App::WINDOW_WIDTH * y) + x]) {
		pixel(x, y, color);
		App::z_buffer[(App::WINDOW_WIDTH * y) + x] = interpolated_reciprocal_w;
	}
}


void Draw::texel(const int x, const int y, Vector4_int const& a, Vector4_int const& b, Vector4_int const& c, 
	Texture2 const& a_uv, Texture2 const& b_uv, Texture2 const& c_uv, const Color* texture)
{
	Vector2_int p(x, y);
	Vector3 weights = Vector3::barycentric_weights(a.to_vec2_int(), b.to_vec2_int(), c.to_vec2_int(), p);

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	float interpolated_u = (a_uv.u / a.w) * alpha + (b_uv.u / b.w) * beta + (c_uv.u / c.w) * gamma;
	float interpolated_v = (a_uv.v / a.w) * alpha + (b_uv.v / b.w) * beta + (c_uv.v / c.w) * gamma;

	float interpolated_reciprocal_w = (1 / a.w) * alpha + (1 / b.w) * beta + (1 / c.w) * gamma;

	interpolated_u /= interpolated_reciprocal_w;
	interpolated_v /= interpolated_reciprocal_w;

	/*float A = alpha * b.w * c.w;
	float B = beta * a.w * c.w;
	float C = gamma * a.w * b.w;

	float interpolated_reciprocal_w = A + B + C;
	float interpolated_u = (a_uv.u * A + b_uv.u * B + c_uv.u * C) / interpolated_reciprocal_w;
	float interpolated_v = (a_uv.v * A + b_uv.v * B + c_uv.v * C) / interpolated_reciprocal_w;*/


	int tex_x = abs(static_cast<int>(interpolated_u * texture_width)) % texture_width;
	int tex_y = abs(static_cast<int>(interpolated_v * texture_height)) % texture_height;

	//interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

	if (interpolated_reciprocal_w > App::z_buffer[(App::WINDOW_WIDTH * y) + x]) {
		pixel(x, y, texture[(tex_y * texture_width) + tex_x]);
		App::z_buffer[(App::WINDOW_WIDTH * y) + x] = interpolated_reciprocal_w;
	}
}

//Draws a dark grey grid of 30 pixel boxes
void Draw::grid()
{
	//vertical lines
	for (int y = 0; y < App::WINDOW_HEIGHT; y++) {
		for (int x = 0; x < App::WINDOW_WIDTH; x += 30) {
			App::display_buffer[App::WINDOW_WIDTH * y + x] = 0xFF101010;
		}
	}

	//horizontal lines
	for (int y = 0; y < App::WINDOW_HEIGHT; y += 30) {
		for (int x = 0; x < App::WINDOW_WIDTH; x++) {
			App::display_buffer[App::WINDOW_WIDTH * y + x] = 0xFF101010;
		}
	}
}