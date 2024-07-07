#ifndef MATRICES_H
#define MATRICES_H

void generateProjectionMatrix(float* matrix, float fov, float aspect, float zNear, float zFar);
void generateTranslationMatrix(float* matrix, float x, float y, float z);
void generateRotationMatrix(float* matrix, float angle, float x, float y, float z);

#endif
