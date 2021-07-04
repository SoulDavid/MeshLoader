#pragma once
#include "math.hpp"
#include <vector>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Color_Buffer.hpp>
#include <cstdlib>
#include "Rasterizer.hpp"

using namespace std;

namespace Engine
{
	using argb::Rgb888;
	using argb::Color_Buffer;

	class Convert_Function
	{
	private:
		typedef Point4f               Vertex;
		typedef vector< Vertex >      Vertex_Buffer;

		typedef Rgb888                Color;
		typedef Point4f               Vertex;
		typedef vector< Vertex >      Vertex_Buffer;
		typedef vector< int    >      Index_Buffer;
		typedef vector< Color  >      Vertex_Colors;


		Vertex_Buffer     original_vertices;
		Index_Buffer      original_indices;
		Vertex_Colors     original_colors;
		Vertex_Buffer     transformed_vertices;
		vector< Point4i > display_vertices;


	public:
		Convert_Function() {};
		void Convert(const aiScene*, int);
		float rand_clamp() { return float(rand() & 0xff) * 0.0039215f; }
	};
}


