#include "Convert_Function.hpp"
#include <Color_Buffer.hpp>

namespace Engine
{
	void Convert_Function::Convert(const aiScene * scene, int id)
	{
        auto mesh = scene->mMeshes[id];

        size_t number_of_vertices = mesh->mNumVertices;

        // Se copian los datos de coordenadas de vértices:

        original_vertices.resize(number_of_vertices);

        for (size_t index = 0; index < number_of_vertices; index++)
        {
            auto& vertex = mesh->mVertices[index];

            original_vertices[index] = Vertex(vertex.x, -vertex.y, vertex.z, 1.f);
        }

        transformed_vertices.resize(number_of_vertices);
        display_vertices.resize(number_of_vertices);

        // Se inicializan los datos de color de los vértices con colores aleatorios:

        original_colors.resize(number_of_vertices);

        for (size_t index = 0; index < number_of_vertices; index++)
        {
            original_colors[index].set(rand_clamp(), rand_clamp(), rand_clamp());
        }

        // Se generan los índices de los triángulos:

        size_t number_of_triangles = mesh->mNumFaces;

        original_indices.resize(number_of_triangles * 3);

        Index_Buffer::iterator indices_iterator = original_indices.begin();

        for (size_t index = 0; index < number_of_triangles; index++)
        {
            auto& face = mesh->mFaces[index];

            assert(face.mNumIndices == 3);              // Una face puede llegar a tener de 1 a 4 índices,
                                                        // pero nos interesa que solo haya triángulos
            auto indices = face.mIndices;

            *indices_iterator++ = int(indices[0]);
            *indices_iterator++ = int(indices[1]);
            *indices_iterator++ = int(indices[2]);
        }
	}
}