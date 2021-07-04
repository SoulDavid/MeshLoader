/**
* @file main.cpp
* Copyright (c) David Martín
* @author David Martín Almazán
* @date 8 de Mayo de 2021
* @section LICENSE
* Licencia MIT
* @section DESCRIPTION
* Script que controla todo el flujo del juego
**/


#include "View.hpp"
#include <SFML/Window.hpp>

using namespace sf;
using namespace Engine;

int main ()
{
    //Medidas de la ventana

    constexpr auto window_width  = 800u;
    constexpr auto window_height = 600u;

    //Se crea la ventana
    Window window(VideoMode(window_width, window_height), "Mesh Loader", Style::Titlebar | Style::Close);

    //Se crea la escena y su camara
    View   view  (window_width, window_height);

    float cx = 10, cy = 0, cz = 15;

    view.camera = new Camera(cx, cy, cz);

    view.camera->Update(0, 0, 0, cx, cy, cz);
    float x = 0;
    float y = 0;
    float z = 0;

    window.setVerticalSyncEnabled (true);

    //El bucle del juego

    bool exit = false;

    do
    {
        Event event;
        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed) exit = true;
            if (event.type == Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    //Si se pulsa W o Flecha Arriba, se rotará la cámara hacia arriba
                case Keyboard::W:
                case Keyboard::Up:
                    if (cz <= 15)
                    {
                        x += 45;
                        z += 45;

                        cx += .5f;
                        cy -= 1.5f;
                    }

                    break;
                    //Si se pulsa S o Flecha Arriba, se rotará la cámara hacia abajo
                case Keyboard::S:
                case Keyboard::Down:
                    if (cz <= 15)
                    {
                        x -= 45;
                        z -= 45;

                        cx -= .5f;
                        cy += 1.5f;
                    }

                    break;
                    //Si se pulsa A o Flecha Derecha, se rotará la cámara hacia derecha
                case Keyboard::A:
                case Keyboard::Left:
                    if (cz <= 15)
                    {
                        y += 45;

                        cx += 1.f;
                    }

                    break;
                    //Si se pulsa D o Flecha Izquierda, se rotará la cámara hacia derecha
                case Keyboard::D:
                case Keyboard::Right:
                    if (cz <= 15)
                    {
                        y -= 45;
                        cx -= 1.f;
                    }


                    break;
                    //Si se pulsa el espacio, se alejará la cámara
                case Keyboard::Space:
                    if(cz < 50.f)
                        cz += 2.f;

                    break;
                    //Si se pulsa la Z, se acercará la cámara
                case Keyboard::Z:

                    if (cz > 12.f)
                        cz -= 2.f;
                    break;
                    //Si se pulsa la L, se moverá a la dcha la camara
                case Keyboard::L:

                    cx += 2;

                    break;
                    //Si se pula la J, se moverá a la izq la camara
                case Keyboard::J:

                    cx -= 2;

                    break;
                    //Si se pulsa K, se moverá hacia abajo la cámara
                case Keyboard::K:

                    cy -= 2;

                    break;
                    //Si se pulsa la I, se moverá hacia arriba la cámara
                case Keyboard::I:
                    cy += 2;

                    break;
                }

                view.camera->Update(x, y, z, cx, cy, cz);

            }
        }

        //Para la visibilidad del fondo, solo se renderizara, si esta en cierta perspectiva, o si la camara esta alejado. 
        //Esto es debido a que se deformaba, y solia salirse de la escena. Al no estar implementado el algorimto de recorte se peta la aplicación.
        if (((cx >= 9.f && cx <= 11.f) && (cy >= -0.5 && cy <= 0.5)))
            view.Scale(true);
        else
        {
            if(cz>=20)
                view.Scale(true);
            else
                view.Scale(false);
        }


        //Se llama al update de la escena
        view.update();

        //Se llama al render de la escena
        view.render ();
        //Se pinta por pantalla
        window.display ();
    }
    while (not exit);

    return 0;
}
