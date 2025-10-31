#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <iostream>
#include <iomanip>

const float PI = 3.14159265359f;
const float EPSILON = 1e-6f;

class Vector3 {
public:
    float x, y, z;

    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const Vector3& v);

    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;
    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);
    Vector3 operator-() const;

    float dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;

    float length() const;
    float lengthSquared() const;
    Vector3 normalized() const;
    void normalize();

    static Vector3 lerp(const Vector3& a, const Vector3& b, float t);
    void print(const std::string& name = "") const;
};

class Matrix4 {
public:
    float m[16];

    Matrix4();
    Matrix4(const Matrix4& mat);

    static Matrix4 identity();
    static Matrix4 translation(float x, float y, float z);
    static Matrix4 translation(const Vector3& v);
    static Matrix4 rotationX(float angle);
    static Matrix4 rotationY(float angle);
    static Matrix4 rotationZ(float angle);
    static Matrix4 scale(float x, float y, float z);
    static Matrix4 scale(const Vector3& s);
    static Matrix4 perspective(float fov, float aspect, float near, float far);
    static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far);
    static Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

    Matrix4 operator*(const Matrix4& mat) const;
    Vector3 operator*(const Vector3& v) const;
    Matrix4& operator*=(const Matrix4& mat);

    void translate(float x, float y, float z);
    void translate(const Vector3& v);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void scale(float x, float y, float z);

    Matrix4 transpose() const;
    Matrix4 inverse() const;
    const float* data() const { return m; }
    void print(const std::string& name = "") const;
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion();
    Quaternion(float w, float x, float y, float z);
    Quaternion(const Quaternion& q);

    static Quaternion identity();
    static Quaternion fromAxisAngle(const Vector3& axis, float angle);

    Quaternion operator*(const Quaternion& q) const;
    Vector3 operator*(const Vector3& v) const;
    Quaternion& operator*=(const Quaternion& q);

    float length() const;
    Quaternion normalized() const;
    void normalize();
    Quaternion conjugate() const;
    Quaternion inverse() const;

    Matrix4 toMatrix4() const;
    static Quaternion fromMatrix4(const Matrix4& mat);

    static Quaternion slerp(const Quaternion& a, const Quaternion& b, float t);

    void print(const std::string& name = "") const;
};

#endif // MATH_H
