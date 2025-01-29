//
// Created by Cooper Stevens on 12/2/24.
//

#ifndef LINALG_H
#define LINALG_H

#include <cmath>
#include <SFML/Graphics.hpp>

//
////
// VECTOR STRUCTURES
////
//


struct Vec2D {
    double x,y;
    Vec2D(double x, double y): x(x), y(y) {}
    Vec2D(): x(0), y(0) {}
    Vec2D operator+(const Vec2D &v) const {
        return Vec2D(x+v.x, y+v.y);
    }
    Vec2D operator-(const Vec2D &v) const {
        return Vec2D(x-v.x, y-v.y);
    }
    Vec2D operator*(const double &s) const {
        return Vec2D(x*s, y*s);
    }
    // cross product of 2d vectors is essentially just the determinant of the matrix with them as column vectors
    double cross(const Vec2D &v) const {
        return x * v.y - y * v.x;
    }
};

struct Vec3D {
    double x,y,z;
    Vec3D(double x, double y, double z): x(x), y(y), z(z) {}
    Vec3D(): x(0), y(0), z(0) {}
    // define operators for addition, subtraction, and scalar multiplication
    Vec3D operator+(const Vec3D &v) const {
        return Vec3D(x+v.x, y+v.y, z+v.z);
    }
    Vec3D operator-(const Vec3D &v) const {
        return Vec3D(x-v.x, y-v.y, z-v.z);
    }
    Vec3D operator*(const double &s) const {
        return Vec3D(x*s, y*s, z*s);
    }
    Vec3D cross(const Vec3D &v) const {
        return Vec3D(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }
    double dot(const Vec3D &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    double length() const {
        return sqrt(x*x + y*y + z*z);
    }

    // small margin for numerical error (not sure if this is necessary, but it is highly unlikely to cause problems)
    bool operator==(const Vec3D& v) const {
        return std::abs(x - v.x) < 0.00001 && std::abs(y - v.y) < 0.00001 && std::abs(z - v.z) < 0.00001;
    }

    bool operator!=(const Vec3D& v) const {
        return !(*this == v);
    }

    bool operator<(const Vec3D& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }

    std::string toString() const {
        return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
    }

};

struct Vec4D {
    double x,y,z,w;
    Vec4D(double x, double y, double z, double w): x(x), y(y), z(z), w(w) {}
    Vec4D(): x(0), y(0), z(0), w(0) {}
    // define operators for addition, subtraction, and scalar multiplication
    Vec4D operator+(const Vec4D &v) const {
        return Vec4D(x+v.x, y+v.y, z+v.z, w+v.w);
    }
    Vec4D operator-(const Vec4D &v) const {
        return Vec4D(x-v.x, y-v.y, z-v.z,w-v.w);
    }
    Vec4D operator*(const double &s) const {
        return Vec4D(x*s, y*s, z*s, w*s);
    }
    double dot(const Vec4D &v) const {
        return x*v.x + y*v.y + z*v.z + w*v.w ;
    }
};


//
////
// MATRIX STRUCTURES
////
//

struct Matrix2x2 {
    Vec2D c1,c2;
    Matrix2x2(Vec2D c1, Vec2D c2): c1(c1), c2(c2){}
    Matrix2x2(): c1(0,0), c2(0,0) {}
    double det() const {
        return c1.x*c2.y-c1.y*c2.x;
    }
};

struct Matrix3x3 {
    // column vector for the matrix
    Vec3D c1,c2,c3;
    Matrix3x3(Vec3D c1, Vec3D c2, Vec3D c3): c1(c1), c2(c2), c3(c3) {}
    Matrix3x3(): c1(0,0,0), c2(0,0,0), c3(0,0,0) {}
    // define operators for addition, subtraction, and scalar multiplication
    Matrix3x3 operator+(const Matrix3x3 &m) const {
        return Matrix3x3(c1+m.c1, c2+m.c2, c3+m.c3);
    }
    Matrix3x3 operator-(const Matrix3x3 &m) const {
        return Matrix3x3(c1-m.c1, c2-m.c2, c3-m.c3);
    }
    Matrix3x3 operator*(const double &s) const {
        return Matrix3x3(c1*s, c2*s, c3*s);
    }
    // matrix-vector multiplication
    Vec3D operator*(const Vec3D &v) const {
        return Vec3D(c1.x * v.x + c2.x * v.y + c3.x * v.z, c1.y * v.x + c2.y * v.y + c3.y * v.z,c1.z * v.x + c2.z * v.y + c3.z * v.z);
    }
    // matrix composition
    Matrix3x3 operator*(const Matrix3x3 &m) const {
        return Matrix3x3(
            *this * m.c1,
            *this * m.c2,
            *this * m.c3
        );
    }
};

struct Matrix4x4 {
    Vec4D c1,c2,c3,c4;
    Matrix4x4(Vec4D c1, Vec4D c2, Vec4D c3, Vec4D c4): c1(c1), c2(c2), c3(c3), c4(c4) {}
    Matrix4x4(): c1(0,0,0,0), c2(0,0,0,0), c3(0,0,0,0), c4(0,0,0,0) {}
    // define operators for addition, subtraction, and scalar multiplication
    Matrix4x4 operator+(const Matrix4x4 &m) const {
        return Matrix4x4(c1+m.c1, c2+m.c2, c3+m.c3, c4+m.c4);
    }
    Matrix4x4 operator-(const Matrix4x4 &m) const {
        return Matrix4x4(c1-m.c1, c2-m.c2, c3-m.c3, c4-m.c4);
    }
    Matrix4x4 operator*(const double &s) const {
        return Matrix4x4(c1*s, c2*s, c3*s, c4*s);
    }
    // matrix-vector multiplication
    Vec4D operator*(const Vec4D &v) const {
        return Vec4D(
            c1.x * v.x + c2.x * v.y + c3.x * v.z + c4.x * v.w,
            c1.y * v.x + c2.y * v.y + c3.y * v.z + c4.y * v.w,
            c1.z * v.x + c2.z * v.y + c3.z * v.z + c4.z * v.w,
            c1.w * v.x + c2.w * v.y + c3.w * v.z + c4.w * v.w
        );
    }

    // matrix composition
    Matrix4x4 operator*(const Matrix4x4 &m) const {
        return Matrix4x4(
            *this * m.c1,
            *this * m.c2,
            *this * m.c3,
            *this * m.c4
        );
    }

};

// putting this after matrices because it is dependent on the Matrix4x4 struct
struct Vec4DTranspose {
    double x,y,z,w;
    Vec4DTranspose(double x, double y, double z, double w): x(x), y(y), z(z), w(w) {}
    Vec4DTranspose(Vec4D v): x(v.x), y(v.y), z(v.z), w(v.w) {}
    Vec4DTranspose operator *(const Matrix4x4 &mat) {
        return Vec4DTranspose(
            mat.c1.x*x + mat.c1.y*y + mat.c1.z*z + mat.c1.w*w,
             mat.c2.x*x + mat.c2.y*y + mat.c2.z*z + mat.c2.w*w,
             mat.c3.x*x + mat.c3.y*y + mat.c3.z*z + mat.c3.w*w,
             mat.c4.x*x + mat.c4.y*y + mat.c4.z*z + mat.c4.w*w);
    }
};


//
////
// PROJECTION FUNCTIONS
////
//

Vec2D getProjectedVector(const Vec3D &v, const sf::Image &image, const Vec3D &cameraPos, double camAngleX, double camAngleY);


#endif









