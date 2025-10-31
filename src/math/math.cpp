#include <iostream>
#include <cmath>

class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    float dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3 cross(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    void normalize() {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length > 0) {
            x /= length;
            y /= length;
            z /= length;
        }
    }
};

class Matrix4 {
public:
    float m[4][4];

    Matrix4() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m[i][j] = (i == j) ? 1 : 0; // Identity matrix
    }

    Matrix4 operator*(const Matrix4& mat) const {
        Matrix4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result.m[i][j] = m[i][0] * mat.m[0][j] + m[i][1] * mat.m[1][j] +
                                  m[i][2] * mat.m[2][j] + m[i][3] * mat.m[3][j];
        return result;
    }

    Vector3 operator*(const Vector3& v) const {
        return Vector3(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        );
    }
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion(float w = 1, float x = 0, float y = 0, float z = 0) : w(w), x(x), y(y), z(z) {}

    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }

    Quaternion slerp(const Quaternion& q, float t) const {
        // Calculate the cosine of the angle between the two quaternions
        float dot = w * q.w + x * q.x + y * q.y + z * q.z;
        const float THRESHOLD = 0.9995f;

        if (dot > THRESHOLD) {
            // If the angle is small, linear interpolation
            Quaternion result = *this + (q - *this) * t;
            result.normalize();
            return result;
        }

        // Acos and Slerp calculations
        dot = std::clamp(dot, -1.0f, 1.0f);
        float theta_0 = std::acos(dot); // angle between the two quaternions
        float theta = theta_0 * t; // angle for the interpolation

        Quaternion q2 = q - (*this * dot); // the second quaternion
        q2.normalize();

        return *this * std::cos(theta) + q2 * std::sin(theta);
    }

    void normalize() {
        float length = std::sqrt(w * w + x * x + y * y + z * z);
        if (length > 0) {
            w /= length;
            x /= length;
            y /= length;
            z /= length;
        }
    }
};
