#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>
#include <iostream>

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

    // Affichage du quaternion, utile pour le débogage
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
};

#endif // QUATERNION_H
