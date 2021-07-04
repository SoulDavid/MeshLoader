/**
* @file Camara.cpp
* Copyright (c) David Martín
* @author David Martín Almazán
* @date 9 de Mayo de 2021
* @section LICENSE
* Licencia MIT
* @section DESCRIPTION
* Script que guarda el movimiento de la camara
**/

#include "Camera.hpp"

namespace Engine
{
	///Constructor por defecto
	Camera::Camera(float x, float y, float z)
	{
		Matrix44 identity(1);
		translation = translate(identity, Vector3f{ x, y, z });
		rotation_x = rotate_around_x(identity, 0);
		rotation_y = rotate_around_y(identity, 0);
		rotation_z = rotate_around_z(identity, 0);
	}

	///Función que recoge el update de la camara
	void Camera::Update(float angle_x, float angle_y, float angle_z, float x, float y, float z)
	{
		Matrix44 identity(1);
		//Recoge las matrices de rotación, por si queremos rotar la camara. Las funciones de rotate cogen la medida en radianes, por lo que lo transformamos multiplicandolo por PI y dividiendolo entre 180.
		rotation_x = rotate_around_x(identity, (angle_x * PI)/180);
		rotation_y = rotate_around_y(identity, (angle_y * PI)/180);
		rotation_z = rotate_around_z(identity, (angle_z * PI)/180);


		translation = translate(identity, Vector3f{ x, y, z });
		//Se calcula la transformación
		transformation = translation * rotation_x * rotation_y * rotation_z;
	}
}