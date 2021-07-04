/**
* @file View.hpp
* Copyright (c) David Martín
* @author David Martín Almazán
* @date 8 de Mayo de 2021
* @section LICENSE
* Licencia MIT
* @section DESCRIPTION
* Script que guarda todos los objetos, y llama a su propio update, render y post render
**/


#ifndef VIEW_HEADER
#define VIEW_HEADER

#include <Color_Buffer.hpp>
#include <cstdlib>
#include "math.hpp"
#include "Rasterizer.hpp"
#include <vector>
#include "Convert_Function.hpp"
#include "Model.h"
#include "Camera.hpp"

namespace Engine
{
    //Declaraciones adelantadas
    using  std::vector;
    using argb::Rgb888;
    using argb::Color_Buffer;

    class Model;

    class View
    {
    private:

        typedef Rgb888                Color;
        typedef Color_Buffer< Color > Color_Buffer;
        typedef Point4f               Vertex;

    public:
        Matrix44 projection;

        Color_Buffer               color_buffer;
        Rasterizer< Color_Buffer > rasterizer;

        ///Array que recoge los modelos que aparecen en la escena
        Model * total_models[12];

        //Medidas de la pantalla
        unsigned width;
        unsigned height;

        //Referencia a la camara
        Camera * camera;

        ///Vectores de luz
        Vector4f light_vector {80, 70, -30, 0};
        Vector4f light_vector_tree{ 5, 50, -20, 0};

        //Angulo con el que gira el sol
        float angle = 0;

        //Constante de PI
        const float PI = 3'1416;

        bool reduceBg = false;

    public:
        ///Constructor por defecto
        View(unsigned, unsigned);
        ///Función que ejecuta el update de todos los objetos
        void update ();
        ///Función que llama al render y post render de todos los objetos
        void render ();

        void Scale(bool);
    };

}

#endif
