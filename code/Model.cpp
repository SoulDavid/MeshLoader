/**
* @file Model.cpp
* Copyright (c) David Martín
* @author David Martín Almazán
* @date 9 de Mayo de 2021
* @section LICENSE
* Licencia MIT
* @section DESCRIPTION
* Script que guarda el funcionamiento de los modelos y sus propiedades
**/

#include "Model.h"

namespace Engine
{
    ///Constructor por defecto del modelo
	Model::Model(char* path, View* given_view, float a, float g, float b, float given_scale, float x, float y, float z, float angle_rotation_x, float angle_rotation_y, bool _isActive)
	{
        //Guardamos el path del archivo 
		mode_path = path;
        //Recogemos una referencia a la escena
		view = given_view;

        ///Importamos el objeto dentro de la escena
		Assimp::Importer importer;
		auto scene = importer.ReadFile
		(
			path,
			aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
		);

        //Si hay una escena creada y el número de meshes es mayor a 0
        if (scene && scene->mNumMeshes > 0)
        {
            //Cogemos el mesh[0] de la escena
            auto mesh = scene->mMeshes[0];

            //Calculamos el numero de vertices
            size_t number_of_vertices = mesh->mNumVertices;

            // Se copian los datos de coordenadas de vértices y de normales:
            original_vertices.resize(number_of_vertices);
            original_normals.resize(number_of_vertices);

            //Se recorre el número de vertices, y por cada vertice, lo guardamos en original vertices. Si tuviese normal (esto es para la iluminación), lo guardamos en un array de normals
            for (size_t index = 0; index < number_of_vertices; index++)
            {
                auto& vertex = mesh->mVertices[index];

                original_vertices[index] = Vertex(vertex.x, -vertex.y, vertex.z, 1.f);

                if (mesh->HasNormals())
                {
                    auto& n = mesh->mNormals[index];
                    original_normals[index] = Vertex(n.x, n.y, n.z, 0);
                }
            }

            //Se inicializan los vertices, normals, y colors
            transformed_vertices.resize(number_of_vertices);
            transformed_normals.resize(number_of_vertices);
            transformed_colors.resize(number_of_vertices);
            display_vertices.resize(number_of_vertices);

            // Se inicializan los datos de color de los vértices con colores aleatorios:

            originals_color.resize(number_of_vertices);


            for (size_t index = 0; index < number_of_vertices; index++)
            {
                //Aqui cambiamos el color
                originals_color[index].set(/*rand_clamp(), rand_clamp(), rand_clamp()*/ a,g,b);
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

        ///Inicialización de las matrices. 
        Matrix44 identity(1);
        scaling = scale(identity, given_scale);
        rotation_x = rotate_around_x(identity, (angle_rotation_x * PI) / 180);
        rotation_y = rotate_around_y(identity, (angle_rotation_y * PI) / 180);
        translation = translate(identity, Vector3f{ x, y, z });
        isActive = _isActive;

	}

    ///Función que recoge las matrices y recoge los vertices que se pintarán por pantalla. Es una función que se llamará antes del Render.
    void Model::Post_Render(int given_width, int given_height)
    {
        Matrix44 identity(1);
        Matrix44 scaling = scale(identity, float(given_width / 2), float(given_height / 2), 100000000.f);
        Matrix44 translation = translate(identity, Vector3f{ float(given_width / 2), float(given_height / 2), 0.f });
        Matrix44 transformation = translation * scaling;

        for (size_t index = 0, number_of_vertices = transformed_vertices.size(); index < number_of_vertices; index++)
        {
            display_vertices[index] = Point4i(transformation * transformed_vertices[index]);
        }
    }

    ///Función que pinta los vertices del modelo, es decir, es la función que pinta el modelo y hace que se vea.
    void Model::Render(bool isRendering)
    {
        if (isRendering)
        {
            for (int* indices = original_indices.data(), *end = indices + original_indices.size(); indices < end; indices += 3)
            {
                if (is_frontface(transformed_vertices.data(), indices))
                {
                    // Se establece el color del polígono a partir del color de su primer vértice:

                    view->rasterizer.set_color(transformed_colors[*indices]);

                    // Se rellena el polígono:

                    view->rasterizer.fill_convex_polygon_z_buffer(display_vertices.data(), indices, indices + 3);
                }
            }

            // Se copia el framebúffer oculto en el framebúffer de la ventana:

            view->color_buffer.blit_to_window();
        }
    }   

    ///Función que calcula la iluminación, y controla el movimiento de vertices.
    void Model::Update(const Vector4f & transformed_light_vector, bool iluminated)
    {

        inverse_matriz = inverse(view->camera->transformation);

        // Creación de la matriz de transformación unificada:

        transformation = inverse_matriz * translation * rotation_y * scaling;

        // Se transforman todos los vértices usando la matriz de transformación resultante:

        for (size_t index = 0, number_of_vertices = original_vertices.size(); index < number_of_vertices; index++)
        {
            // Se multiplican todos los vértices originales con la matriz de transformación y
            // se guarda el resultado en otro vertex buffer:

            Vertex& vertex = transformed_vertices[index] = view->projection * transformation * original_vertices[index];

            Vertex& n = transformed_normals[index] = transformation * original_normals[index];

            //Producto escalar entre el vector de luz y el vector normal
            float intensity = glm::dot(normalize(transformed_light_vector) , normalize(n));

            //Se clampea el resultado
            if (intensity < 0.f)
                intensity = 0.f;
            else if (intensity > 1.f)
                intensity = 1.f;

            //Se aplica la iluminacion a cada uno de los componentes RGB.
            //IMPORTANTE: los componentes de los original colors deben ser divididos entre 255 para que no sea o blanco o negro.
            if (iluminated)
            {
                transformed_colors[index].set_red(float(originals_color[index].red())/255.f * intensity);
                transformed_colors[index].set_green(float(originals_color[index].green())/255.f * intensity);
                transformed_colors[index].set_blue(float(originals_color[index].blue())/255.f * intensity);
            }
            else
            {
                transformed_colors[index] = originals_color[index];
            }



            // La matriz de proyección en perspectiva hace que el último componente del vector
            // transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:

            float divisor = 1.f / vertex.w;

            vertex.x *= divisor;
            vertex.y *= divisor;
            vertex.z *= divisor;
            vertex.w = 1.f;
        }
    }

    bool Model::is_frontface(const Vertex* const projected_vertices, const int* const indices)
    {
        const Vertex& v0 = projected_vertices[indices[0]];
        const Vertex& v1 = projected_vertices[indices[1]];
        const Vertex& v2 = projected_vertices[indices[2]];

        // Se asumen coordenadas proyectadas y polígonos definidos en sentido horario.
        // Se comprueba a qué lado de la línea que pasa por v0 y v1 queda el punto v2:

        return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) < 0.f);
    }
}
