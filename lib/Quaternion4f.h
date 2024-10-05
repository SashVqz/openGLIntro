#pragma once

#include "Vector4f.h"
#include "Mat4x4f.h"

namespace libMath {

	inline vector4f make_quaternion(float x, float y, float z, float angle) {
        vector4f quaternion;
        float half_angle = angle / 2.0f;
        float sin_half_angle = sin(half_angle);

        quaternion.x = x * sin_half_angle;
        quaternion.y = y * sin_half_angle;
        quaternion.z = z * sin_half_angle;
        quaternion.w = cos(half_angle);

        return quaternion;
    }

    inline mat4x4f make_rotate(vector4f quaternion) {
        mat4x4f rotation_matrix;

        float x = quaternion.x;
        float y = quaternion.y;
        float z = quaternion.z;
        float w = quaternion.w;

        rotation_matrix.data2D[0][0] = 1 - 2 * y * y - 2 * z * z;
        rotation_matrix.data2D[0][1] = 2 * x * y - 2 * z * w;
        rotation_matrix.data2D[0][2] = 2 * x * z + 2 * y * w;
        rotation_matrix.data2D[0][3] = 0;

        rotation_matrix.data2D[1][0] = 2 * x * y + 2 * z * w;
        rotation_matrix.data2D[1][1] = 1 - 2 * x * x - 2 * z * z;
        rotation_matrix.data2D[1][2] = 2 * y * z - 2 * x * w;
        rotation_matrix.data2D[1][3] = 0;

        rotation_matrix.data2D[2][0] = 2 * x * z - 2 * y * w;
        rotation_matrix.data2D[2][1] = 2 * y * z + 2 * x * w;
        rotation_matrix.data2D[2][2] = 1 - 2 * x * x - 2 * y * y;
        rotation_matrix.data2D[2][3] = 0;

        rotation_matrix.data2D[3][0] = 0;
        rotation_matrix.data2D[3][1] = 0;
        rotation_matrix.data2D[3][2] = 0;
        rotation_matrix.data2D[3][3] = 1;

        return rotation_matrix;
    }

}