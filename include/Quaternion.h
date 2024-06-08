#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>
#include <iostream>
#include "Block.h"

class Quaternion {
public:
    float w, x, y, z;

    // Constructeur par défaut et paramétrique
    Quaternion(float w = 1.0, float x = 0.0, float y = 0.0, float z = 0.0);

    // Addition de deux quaternions
    Quaternion operator+(const Quaternion& q) const;

    // Multiplication de deux quaternions
    Quaternion operator*(const Quaternion& q) const;

    // Conjugaison du quaternion, retournant le quaternion conjugé
    Quaternion conjugate() const;

    // Calcule la norme du quaternion
    float norm() const;

    // Normalise le quaternion
    Quaternion normalize() const;

    // Calcule le produit scalaire de deux quaternions
    float dot(const Quaternion& q) const;

    // Passage d'un quaternion en matrice
    void to4x4Matrix(float matrix[16]) const;

    // Passage d'une matrice en quaternion
    static Quaternion from4x4Matrix(const float matrix[16]);

    // Multiplication de deux matrices 4x4
    static void multiplyMatrices4x4(const float mat1[16], const float mat2[16], float result[16]);

    // Conversion d'un quaternion en matrice de rotation 3x3
    void toRotationMatrix3x3(float matrix[9]) const;

    // Conversion d'une matrice de rotation 3x3 en quaternion
    static Quaternion fromRotationMatrix3x3(const float matrix[9]);

    // Rotation d'un point par un quaternion
    static void rotatePointWithQuaternion(float point[3], const Quaternion& q);

    // Affichage du quaternion, utile pour le débogage
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);


};

#endif // QUATERNION_H
