#include "include/Quaternion.h"

// Constructeur de la classe Quaternion
Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

// Opérateur d'addition de deux quaternions
Quaternion Quaternion::operator+(const Quaternion& q) const {
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

// Opérateur de multiplication de deux quaternions
Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
    );
}

// Retourne le quaternion conjugué
Quaternion Quaternion::conjugate() const {
    return Quaternion(w, -x, -y, -z);
}

// Calcule la norme du quaternion
float Quaternion::norm() const {
    return std::sqrt(w * w + x * x + y * y + z * z);
}

// Normalise le quaternion
Quaternion Quaternion::normalize() const {
    float n = norm();
    return Quaternion(w / n, x / n, y / n, z / n);
}

// Calcule le produit scalaire avec un autre quaternion
float Quaternion::dot(const Quaternion& q) const {
    return w * q.w + x * q.x + y * q.y + z * q.z;
}

// Affichage d'un quaternion
std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
    os << "(" << q.w << " + " << q.x << "i + " << q.y << "j + " << q.z << "k)";
    return os;
}
