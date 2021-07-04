/**
* @file Camara.hpp
* Copyright (c) David Martín
* @author David Martín Almazán
* @date 9 de Mayo de 2021
* @section LICENSE
* Licencia MIT
* @section DESCRIPTION
* Script que guarda el movimiento de la camara
**/

#pragma once
#include "math.hpp"

namespace Engine
{
	class Camera
	{
	public:
		///Las matrices
		Matrix44 rotation_x;
		Matrix44 rotation_y;
		Matrix44 rotation_z;
		Matrix44 translation;
		Matrix44 transformation;

		//Constante de PI
		float const PI = 3'1416;

	public:
		///Constructor por defecto
		Camera(float, float, float);

		///Función que recoge el update de la camara
		void Update(float, float, float, float, float, float);
	};
}


