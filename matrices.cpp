#include "matrices.h"
#include <cmath>

void generateProjectionMatrix(float* matrix, float fov, float aspect, float zNear, float zFar) {
    float f = 1.0f / tan(fov / 2.0f);
    matrix[0] = f / aspect;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    matrix[4] = 0.0f;
    matrix[5] = f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = (zFar + zNear) / (zNear - zFar);
    matrix[11] = -1.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = (2.0f * zFar * zNear) / (zNear - zFar);
    matrix[15] = 0.0f;
}

void generateTranslationMatrix(float* matrix, float x, float y, float z) {
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;

    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;
    matrix[15] = 1.0f;
}

void generateRotationMatrix(float* matrix, float angle, float x, float y, float z) {
    float c = cos(angle);
    float s = sin(angle);
    float omc = 1.0f - c;

    matrix[0] = x * x * omc + c;
    matrix[1] = y * x * omc + z * s;
    matrix[2] = x * z * omc - y * s;
    matrix[3] = 0.0f;

    matrix[4] = x * y * omc - z * s;
    matrix[5] = y * y * omc + c;
    matrix[6] = y * z * omc + x * s;
    matrix[7] = 0.0f;

    matrix[8] = x * z * omc + y * s;
    matrix[9] = y * z * omc - x * s;
    matrix[10] = z * z * omc + c;
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}
