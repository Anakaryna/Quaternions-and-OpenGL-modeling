#include "include/Quaternion.h"

Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

Quaternion Quaternion::operator+(const Quaternion& q) const {
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
    );
}

Quaternion Quaternion::conjugate() const {
    return Quaternion(w, -x, -y, -z);
}

float Quaternion::norm() const {
    return std::sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::normalize() const {
    float n = norm();
    return Quaternion(w / n, x / n, y / n, z / n);
}

float Quaternion::dot(const Quaternion& q) const {
    return w * q.w + x * q.x + y * q.y + z * q.z;
}

void Quaternion::to4x4Matrix(float matrix[16]) const {
    matrix[0] = 1 - 2*y*y - 2*z*z;
    matrix[1] = 2*x*y - 2*z*w;
    matrix[2] = 2*x*z + 2*y*w;
    matrix[3] = 0;
    matrix[4] = 2*x*y + 2*z*w;
    matrix[5] = 1 - 2*x*x - 2*z*z;
    matrix[6] = 2*y*z - 2*x*w;
    matrix[7] = 0;
    matrix[8] = 2*x*z - 2*y*w;
    matrix[9] = 2*y*z + 2*x*w;
    matrix[10] = 1 - 2*x*x - 2*y*y;
    matrix[11] = 0;
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}


Quaternion Quaternion::from4x4Matrix(const float matrix[16]) {
    float a = matrix[0];  // a
    float b = matrix[4];  // b
    float c = matrix[8];  // c
    float d = matrix[12]; // d
    return Quaternion(a, b, c, d);
}



void Quaternion::multiplyMatrices4x4(const float mat1[16], const float mat2[16], float result[16]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i * 4 + j] = 0;
            for (int k = 0; k < 4; ++k) {
                result[i * 4 + j] += mat1[i * 4 + k] * mat2[k * 4 + j];
            }
        }
    }
}

void Quaternion::toRotationMatrix3x3(float matrix[9]) const {
    float a = w, b = x, c = y, d = z;
    matrix[0] = 1 - 2*c*c - 2*d*d;
    matrix[1] = 2*b*c - 2*a*d;
    matrix[2] = 2*b*d + 2*a*c;
    matrix[3] = 2*b*c + 2*a*d;
    matrix[4] = 1 - 2*b*b - 2*d*d;
    matrix[5] = 2*c*d - 2*a*b;
    matrix[6] = 2*b*d - 2*a*c;
    matrix[7] = 2*c*d + 2*a*b;
    matrix[8] = 1 - 2*b*b - 2*c*c;
}

Quaternion Quaternion::fromRotationMatrix3x3(const float matrix[9]) {
    float trace = matrix[0] + matrix[4] + matrix[8];
    float w, x, y, z;

    if (trace > 0) {
        float s = 0.5f / std::sqrt(trace + 1.0f);
        w = 0.25f / s;
        x = (matrix[7] - matrix[5]) * s;
        y = (matrix[2] - matrix[6]) * s;
        z = (matrix[3] - matrix[1]) * s;
    } else {
        if (matrix[0] > matrix[4] && matrix[0] > matrix[8]) {
            float s = 2.0f * std::sqrt(1.0f + matrix[0] - matrix[4] - matrix[8]);
            w = (matrix[7] - matrix[5]) / s;
            x = 0.25f * s;
            y = (matrix[1] + matrix[3]) / s;
            z = (matrix[2] + matrix[6]) / s;
        } else if (matrix[4] > matrix[8]) {
            float s = 2.0f * std::sqrt(1.0f + matrix[4] - matrix[0] - matrix[8]);
            w = (matrix[2] - matrix[6]) / s;
            x = (matrix[1] + matrix[3]) / s;
            y = 0.25f * s;
            z = (matrix[5] + matrix[7]) / s;
        } else {
            float s = 2.0f * std::sqrt(1.0f + matrix[8] - matrix[0] - matrix[4]);
            w = (matrix[3] - matrix[1]) / s;
            x = (matrix[2] + matrix[6]) / s;
            y = (matrix[5] + matrix[7]) / s;
            z = 0.25f * s;
        }
    }

    return Quaternion(w, x, y, z);
}



void Quaternion::rotatePointWithQuaternion(float point[3], const Quaternion& q) {
    Quaternion p(0, point[0], point[1], point[2]);
    Quaternion qConj = q.conjugate();
    Quaternion result = q * p * qConj;
    point[0] = result.x;
    point[1] = result.y;
    point[2] = result.z;
}

void Quaternion::rotatePoint(float point[3]) const {
    Quaternion p(0.0f, point[0], point[1], point[2]);
    Quaternion qConjugate = this->conjugate();
    Quaternion result = (*this) * p * qConjugate;

    point[0] = result.x;
    point[1] = result.y;
    point[2] = result.z;
}


std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
    os << "(" << q.w << " + " << q.x << "i + " << q.y << "j + " << q.z << "k)";
    return os;
}



void applyShear(float shX, float shY, float shZ)
{
    float shearMatrix[16] = {
            1.0f, shX, shY, 0.0f,
            shX, 1.0f, shZ, 0.0f,
            shY, shZ, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    glMultMatrixf(shearMatrix);
}

// Shear et Scale avec Quaternions
void Quaternion::shearAndScalePointandTranslate(float *point, float shX, float shY, float shZ, float scaleX,
                                                float scaleY, float scaleZ, float transX, float transY, float transZ) {
    float x = point[0];
    float y = point[1];
    float z = point[2];

    // Appliquer Scale
    x *= scaleX;
    y *= scaleY;
    z *= scaleZ;

    // Appliquer Shearing
    float newX = x + shX * y + shY * z;
    float newY = y + shX * x + shZ * z;
    float newZ = z + shY * x + shZ * y;

    // Applpliquer Translation
    point[0] = newX + transX;
    point[1] = newY + transY;
    point[2] = newZ + transZ;
}

