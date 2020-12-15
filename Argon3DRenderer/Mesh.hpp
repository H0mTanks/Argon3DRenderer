#pragma once
#include <vector>
#include "Vector.hpp"
#include "Triangle.hpp"

#define N_MESH_VERTICES 8

std::vector<Vector3> mesh_vertices = {
   {-1 , -1, -1},
   {-1 ,  1, -1},
   { 1 ,  1, -1},
   { 1 , -1, -1},
   { 1 ,  1,  1},
   { 1 , -1,  1},
   {-1 ,  1,  1},
   {-1 , -1,  1},
};

#define N_MESH_FACES (6 * 2)
std::vector<Face> mesh_faces = {
	{1, 2, 3},
	{1, 3, 4},
	{4, 3, 5},
	{4, 5, 6},
	{6, 5, 7},
	{6, 7, 8},
	{8, 7, 2},
	{8, 2, 1},
	{2, 7, 5},
	{2, 5, 3},
	{6, 8, 1},
	{6, 1, 4},
};