/**
* @file Camara.hpp
* Copyright (c) angel.rodriguez@esne.edu
* @author angel.rodriguez@esne.edu
* @date 2016.01 - 2021.04
* @section LICENSE
* Licencia MIT
* @section DESCRIPTION
* Script que guarda las propiedades de los vectores, ya sea Point4f, 3f, 2f
**/

#ifndef MATH_HEADER
#define MATH_HEADER

    #include <glm/glm.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <glm/gtx/matrix_decompose.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    namespace Engine
    {

        typedef glm::ivec2 Vector2i;
        typedef glm::ivec3 Vector3i;
        typedef glm::ivec4 Vector4i;

        typedef glm::fvec2 Vector2f;
        typedef glm::fvec3 Vector3f;
        typedef glm::fvec4 Vector4f;

        typedef glm::quat Quaternion;

        typedef glm::mat2 Matrix22;
        typedef glm::mat3 Matrix33;
        typedef glm::mat4 Matrix44;

        typedef glm::ivec2 Point2i;
        typedef glm::ivec3 Point3i;
        typedef glm::ivec4 Point4i;

        typedef glm::fvec2 Point2f;
        typedef glm::fvec3 Point3f;
        typedef glm::fvec4 Point4f;

        template< class CLASS >
        inline const float * get_values (const CLASS & object)
        {
            return glm::value_ptr(object);
        }

        inline Matrix44 inverse (const Matrix44 & matrix)
        {
            return glm::inverse (matrix);
        }

        inline Matrix44 transpose (const Matrix44 & matrix)
        {
            return glm::transpose (matrix);
        }

        inline Matrix44 translate (const Matrix44 & matrix, const Vector3f & displacement)
        {
            return glm::translate (matrix, displacement);
        }

        inline Matrix44 scale (const Matrix44 & matrix, float scale)
        {
            return glm::scale (matrix, Vector3f(scale, scale, scale));
        }

        inline Matrix44 scale (const Matrix44 & matrix, float scale_x, float scale_y, float scale_z)
        {
            return glm::scale (matrix, Vector3f(scale_x, scale_y, scale_z));
        }

        inline Matrix44 rotate_around_x (const Matrix44 & matrix, float angle)
        {
            return glm::rotate (matrix, angle, glm::vec3(1.f, 0.f, 0.f));
        }

        inline Matrix44 rotate_around_y (const Matrix44 & matrix, float angle)
        {
            return glm::rotate (matrix, angle, glm::vec3(0.f, 1.f, 0.f));
        }

        inline Matrix44 rotate_around_z (const Matrix44 & matrix, float angle)
        {
            return glm::rotate (matrix, angle, glm::vec3(0.f, 0.f, 1.f));
        }

        inline Matrix44 perspective (float fov, float near_d, float far_d, float aspect_ratio)
        {
            return glm::perspective (fov, aspect_ratio, near_d, far_d);
        }

        inline Vector2f extract_translation (const Matrix33 & transformation)
        {
            const  Vector3f & translation = transformation[2];
            return Vector2f(translation[0], translation[1]);
        }

        inline Vector3f extract_translation (const Matrix44 & transformation)
        {
            const  Vector4f & translation = transformation[3];
            return Vector3f(translation[0], translation[1], translation[2]);
        }

        inline Quaternion extract_rotation (const Matrix44 & transformation)
        {
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;

            glm::decompose (transformation, scale, rotation, translation, skew, perspective);

            return rotation;
        }

        inline Matrix44 look_at (const Point3f & from, const Point3f & to)
        {
            return glm::lookAt (from, to, Vector3f(0.f, 1.f, 0.f));
        }

    }

#endif
