//
// Created by Cooper Stevens on 12/5/24.
//
#include <set>
#include <tuple>
#include <array>
#include <algorithm>

#include "Rasterizer.h"

#include <iostream>
#include <unordered_set>

// fills in all pixels within a triangle. uses the barycentric coordinates method to determine if pixels are bounded by the triangle
void fillTriangle(const Vec2D &A, const Vec2D &B, const Vec2D &C, const sf::Color &color, sf::Image &image) {
    // we're trying to find the coordinates that creates the smallest rectangle that bounds the triangle completely

    // we can't color in a pixel with a negative position, so set the lowest possible min to 0
    int minX = std::max(0, static_cast<int>(std::min({A.x, B.x, C.x})));
    // size of the window's x component in case the triangle takes up the entire screen
    int maxX = std::min(static_cast<int>(image.getSize().x - 1), static_cast<int>(std::max({A.x, B.x, C.x})));
    int minY = std::max(0, static_cast<int>(std::min({A.y, B.y, C.y})));
    int maxY = std::min(static_cast<int>(image.getSize().y - 1), static_cast<int>(std::max({A.y, B.y, C.y})));

    // area using determinant
    double area = Matrix2x2(B-A, C-A).det();

    // iterate through the bounds the triangle lies in
    for (int i = minX; i <= maxX; i++) {
        for (int j = minY; j <= maxY; j++) {
            // get barycentric coordinates
            double w1 = ((B.x - i) * (C.y - j) - (C.x - i) * (B.y - j)) / area;
            double w2 = ((C.x - i) * (A.y - j) - (A.x - i) * (C.y - j)) / area;
            double w3 = 1.0 - w1 - w2;
            // check if the point is inside the triangle
            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                image.setPixel(i, j, color);
            }
        }
    }
}


void rasterizeMesh(const Mesh &mesh, const Vec3D &cam, sf::Image &image, Vec3D lightSource, double camAngleX, double camAngleY) {
    std::vector<Triangle3D> rasterizableTris;

    for (auto &tri : mesh.surfaceTriangles) {
        Vec3D centroid = (tri.a + tri.b + tri.c) * (1.0 / 3.0);
        // vector from the camera to the centroid
        Vec3D viewVector = centroid - cam;

        if (tri.normal.dot(viewVector) > -0.0001) {
            // cull triangle if it is facing away from the camera
            // (backface culling)
            continue;
        }

        if (tri.normal.dot(viewVector) < 0.0001) {
            // normalize light source position vector
            lightSource = lightSource * (1.0 / lightSource.length());
            // get "lighting factor". we'll use this to determine how much to shade in triangles
            double lightingFactor = std::max(0.0, tri.normal.dot(lightSource));

            // get color. color gets darker as dot product decreases (color gets darker as angle between the triangle's normal and the light source increases)
            sf::Uint8 gray = static_cast<sf::Uint8>(lightingFactor * 255);
            tri.color = sf::Color(gray, gray, gray);

            // put triangle in vector to be rasterized
            rasterizableTris.push_back(tri);
        }
    }

    // sort triangles by depth
    // we'll have visual bugs if triangles that are behind other triangles are rasterized first
    std::sort(rasterizableTris.begin(), rasterizableTris.end(), [cam](const Triangle3D &tri1, const Triangle3D &tri2) {
        double z1 = ((tri1.a + tri1.b + tri1.c) * (1.0 / 3.0) - cam).length();
        double z2 = ((tri2.a + tri2.b + tri2.c) * (1.0 / 3.0) - cam).length();
        return z1 > z2;
    });

    // draw each projected triangle
    for (const auto &tri : rasterizableTris) {
        // project vertices onto a 2d plane

        Vec2D projectedVert1 = getProjectedVector(tri.a, image, cam, camAngleX, camAngleY);
        Vec2D projectedVert2 = getProjectedVector(tri.b, image, cam, camAngleX, camAngleY);
        Vec2D projectedVert3 = getProjectedVector(tri.c, image, cam, camAngleX, camAngleY);

        projectedVert1.y *= 1;
        projectedVert2.y *= 1;
        projectedVert3.y *= 1;

        fillTriangle(projectedVert1, projectedVert2, projectedVert3, tri.color, image);
    }

}

Vec3D computeMeshCenter(const Mesh& mesh) {
    Vec3D weightedSum(0, 0, 0);
    double totalArea = 0.0;

    for (const auto& tri : mesh.surfaceTriangles) {
        Vec3D edge1 = tri.b - tri.a;
        Vec3D edge2 = tri.c - tri.a;
        double area = edge1.cross(edge2).length() * 0.5;
        Vec3D centroid = (tri.a + tri.b + tri.c) * (1.0 / 3.0);

        weightedSum = weightedSum + (centroid * area);
        totalArea += area;
    }

    // avoid runtime error
    if (totalArea == 0.0) return Vec3D(0, 0, 0);

    // find the weighted average
    Vec3D meshCenter = weightedSum * (1.0 / totalArea);
    return meshCenter;
}

// this works for relatively simple meshes and is dependent on all vectors from mesh's centroid to the triangles' centroids facing outwards (if the mesh centroid is outside of the mesh, this will not work)
// typically mesh triangles are defined in the .txt files in the /inputs directory in such a way that their normals are facing outwards.
// (this occurs when the triangle vertices are defined in counterclockwise order when looking directly at the triangle from outside the mesh)
// but some input files do not follow this pattern, so this function ensures a mesh's triangle normals are oriented properly
void ensureNormalsFaceOutward(Mesh& mesh) {
    Vec3D meshCenter = computeMeshCenter(mesh);

    for (auto& tri : mesh.surfaceTriangles) {

        Vec3D normal = tri.normal;

        // check for triangles wth 0 area (colinear vertices)
        double area = normal.length() * 0.5;
        if (area == 0.0) {
            tri.normal = Vec3D(0, 0, 0);
            continue;
        }
        Vec3D centroid = (tri.a + tri.b + tri.c) * (1.0 / 3.0);

        // vec from mesh center to triangle centroid
        Vec3D toCentroid = centroid - meshCenter;

        // check if the normal is pointing inward or outward
        double dotProduct = normal.dot(toCentroid);
        if (dotProduct < 0) normal = normal * (-1.0);

        tri.normal = normal;
    }
}





