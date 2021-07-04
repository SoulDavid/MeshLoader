/**
* @file View.cpp
* Copyright (c) David Martín
* @author David Martín Almazán
* @date 8 de Mayo de 2021
* @section LICENSE
* Licencia MIT
* @section DESCRIPTION
* Script que guarda todos los objetos, y llama a su propio update, render y post render
**/

#include <cassert>
#include <cmath>
#include "math.hpp"
#include "View.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
    ///Constructor por defecto
    View::View(unsigned width, unsigned height)
    :
        width       (width ),
        height      (height),
        color_buffer(width, height),
        rasterizer  (color_buffer )
    {
        //Inicializamos la matriz de proyección
        projection = perspective(20, 1, 15, float(width) / height);

        //Creacion de modelos
        //char* path, View* given_view, float a, float g, float b, float given_scale, float x, float y, float z, float angle_rotation_x, float angle_rotation_y
        //Arbol
        total_models[0] = new Model("../../shared/assets/Lowpoly_tree_sample.obj", this, 250.f, 150.f, 0.f, 0.1f, 3.f, 2.f, -15.f, 0.f, 0.f, true);

        //Conejo
        total_models[1] = new Model("../../shared/assets/stanford-bunny.obj", this, 15.f, 0.f, 0.f, 1.f, 0.f, 1.f, -10.f, 0.f, 0.f, true);

        //Arboles
        total_models[2] = new Model("../../shared/assets/Lowpoly_tree_sample.obj", this, 250.f, 150.f, 0.f, 0.1f, -2.5f, 1.f, -5.f, 0.f, 0.f, true);
        total_models[3] = new Model("../../shared/assets/Lowpoly_tree_sample.obj", this, 250.f, 150.f, 0.f, 0.075f, -1.f, 0.f, -10.f, 0.f, 0.f, true);
        total_models[4] = new Model("../../shared/assets/Lowpoly_tree_sample.obj", this, 250.f, 150.f, 0.f, 0.05f, 10.f, 0.f, -10.f, 0.f, 0.f, true);

        //Montañas
        total_models[5] = new Model("../../shared/assets/mountain.obj", this, 125.f, 150.f, 0.f, 50.f, -10.f, 0.f, -30.f, 0.f, 270.f, true);
        total_models[6] = new Model("../../shared/assets/mountain.obj", this, 125.f, 150.f, 0.f, 60.f, 10.f, 0.f, -30.f, 0.f, 270.f, true);
        total_models[7] = new Model("../../shared/assets/mountain.obj", this, 125.f, 150.f, 0.f, 60.f, 0.f, 0.f, -25.f, 0.f, 180.f, true);
        total_models[8] = new Model("../../shared/assets/mountain.obj", this, 125.f, 150.f, 0.f, 60.f, 25.f, 0.f, -40.f, 0.f, 270.f, true);
        total_models[9] = new Model("../../shared/assets/mountain.obj", this, 125.f, 150.f, 0.f, 60.f, -25.f, 0.f, -40.f, 0.f, 270.f, true);

        //Sol
        total_models[10] = new Model("../../shared/assets/Sun.obj", this, 141.f, 151.f, 0.f, 0.2f, 10.f, -3.f, -40.f, 0.f, 0.f, true);

        //Fondo
        total_models[11] = new Model("../../shared/assets/floor.obj", this, 205.f, 100.f, 50.f, 0.01f, 1.f, 0.f, -100.f, 0.f, 0.f, true);


    }

    ///Función que ejecuta el update de todos los objetos
    void View::update ()
    {
        //Hacemos el update de todos los elementos. Le pasamos el vector de luz, y si le afecta o no esta. 
        total_models[0]->Update(light_vector_tree, true);
        total_models[1]->Update(light_vector, true);
        total_models[2]->Update(light_vector_tree, true);
        total_models[3]->Update(light_vector_tree, true);
        total_models[4]->Update(light_vector_tree, true);
        total_models[5]->Update(light_vector, true);
        total_models[6]->Update(light_vector, true);
        total_models[7]->Update(light_vector, true);
        total_models[8]->Update(light_vector, true);
        total_models[9]->Update(light_vector, true);
        total_models[10]->Update(light_vector, false);
        total_models[11]->Update(light_vector, false);

        //El vector de luz cambiará mediante el movimiento del sol
        light_vector = { light_vector.x + cos((angle * PI) / 180) , light_vector.y, light_vector.z + sin(((angle * PI) / 180)), 0 };
        light_vector_tree = { light_vector_tree.x + cos((angle * PI) / 180) , light_vector_tree.y, light_vector_tree.z + sin(((angle * PI) / 180)), 0 };

        //Updateamos la posición del sol, para que vaya orbitando alrededor de la montaña
        total_models[10]->translation = translate(total_models[10]->translation, { cos(((angle * PI) / 180)), 0, sin(((angle*PI)/180)) });

        //Vamos bajando el angle
        angle -= 45;

    }

    void View::Scale(bool _isActive)
    {
        //total_models[11]->translation = translate(total_models[11]->translation, { 205.f, 100.f, 50.f });
        total_models[11]->isActive = _isActive;
    }

    ///Función que llama al render y post render de todos los objetos
    void View::render()
    {
        //Se recorre un bucle que realiza el Post_Render de cada elemento
        for (int i = 0; i < 12; ++i)
        {
            total_models[i]->Post_Render(width, height);
        }

        // Se borra el framebúffer y se dibujan los triángulos:
        rasterizer.clear();

        //Se recorre un bucle que realiza el render de cada elemento.
        for (int i = 0; i < 12; ++i)
        {
            total_models[i]->Render(total_models[i]->isActive);
        }
    }

}
