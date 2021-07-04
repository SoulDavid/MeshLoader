/**
* @file Model.h
* Copyright (c) David Martín
* @author David Martín Almazán
* @date 9 de Mayo de 2021
* @section LICENSE
* Licencia MIT
* @section DESCRIPTION
* Script que guarda el funcionamiento de los modelos y sus propiedades
**/

#pragma once
#include <vector>
#include "math.hpp"
#include <Color_Buffer.hpp>
#include "Rasterizer.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "View.hpp"

using namespace std;

namespace Engine
{
    ///Declaraciones adelantas
    using  std::vector;
    using argb::Rgb888;
    using argb::Color_Buffer;

    class View;

    class Model
    {
    public:
        //Path del archivo del modelo
        char* mode_path;

        //Recoge referencia a la escena
        View* view;

        //Variables de los atributos de los vertices
        typedef Point4f               Vertex;
        typedef vector< Vertex >      Vertex_Buffer;

        typedef Rgb888                Color;
        typedef vector< Color  >      Vertex_Color;

        typedef vector< int    >      Index_Buffer;

#pragma region Atributo de vertices
        Vertex_Buffer original_vertices;
        Vertex_Buffer original_normals;
        Vertex_Color originals_color;
        Index_Buffer original_indices;
#pragma endregion

#pragma region Transformación de los atributos de los vertices
        Vertex_Buffer transformed_vertices;
        vector<Point4i> display_vertices;
        Vertex_Buffer transformed_normals;
        Vertex_Color transformed_colors;
#pragma endregion

        //Constante de PI
        const float PI = 3'1416;

        bool isActive;

    public:
        //Matrices
        Matrix44 scaling;
        Matrix44 rotation_x;
        Matrix44 rotation_y;
        Matrix44 translation;
        Matrix44 inverse_matriz;
        Matrix44 transformation;

    public: 
        ///Constructor por defecto del modelo
        Model(char*, View*, float, float, float, float, float, float, float, float, float, bool);
        float rand_clamp() { return float(rand() & 0xff) * 0.0039215f; }
        ///Función que recoge las matrices y recoge los vertices que se pintarán por pantalla. Es una función que se llamará antes del Render.
        void Post_Render(int, int);
        ///Función que pinta los vertices del modelo, es decir, es la función que pinta el modelo y hace que se vea.
        void Render(bool);
        ///Función que calcula la iluminación, y controla el movimiento de vertices.
        void Update(const Vector4f &, bool);
        bool is_frontface(const Vertex* const, const int* const);
        //function to calculate dot product of two vectors
        int dot_product(Vector3f, Vertex);

    };
}


