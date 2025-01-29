//
// Created by Cooper Stevens on 12/5/24.
//

#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <vector>
#include "LinAlg.h"


struct Triangle3D {
    Vec3D a, b, c, normal;
    mutable sf::Color color = sf::Color(255,255,255);
    Triangle3D(Vec3D a, Vec3D b, Vec3D c) : a(a), b(b), c(c) {
        normal = (b-a).cross(c-a);
        normal = normal * (1.0/normal.length());
    }
};

struct Mesh {
    std::vector<Triangle3D> surfaceTriangles;
    Vec3D center;
    explicit Mesh(std::vector<Triangle3D> const &surfaceTriangles) : surfaceTriangles(surfaceTriangles) {};
    void translate(const Vec3D &t) {
        for (auto &tri : this->surfaceTriangles) {
            tri.a = tri.a + t;
            tri.b = tri.b + t;
            tri.c = tri.c + t;
        }
    }
};

void rasterizeMesh(const Mesh &mesh, const Vec3D &cam, sf::Image &image, Vec3D lightSource, double camAngleX, double camAngleY);

inline void fillTriangle(const Vec2D &A, const Vec2D &B, const Vec2D &C, const sf::Color &color, sf::Image &image);
#endif

void ensureNormalsFaceOutward(Mesh& mesh);




