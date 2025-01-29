//
// Created by Cooper Stevens on 12/5/24.
//

#include "LinAlg.h"

#include <iostream>

// generates a perspective projection matrix
// a perspective projection matrix is a linear operator that will allow us to project a 3d point into 2d space
// fov in radians
Matrix4x4 getPerspectiveProjectionMatrix(const double &fov, const sf::Image &image, double nearPlane, double farPlane) {
    double ar = static_cast<double>(image.getSize().x) / image.getSize().y;
    double scale = 1.0 / tan(fov * 0.5);
    double nf = 1.0 / (nearPlane - farPlane);

    return {
        Vec4D(scale / ar, 0, 0, 0),
        Vec4D(0, scale, 0, 0),
        Vec4D(0, 0, (farPlane + nearPlane) * nf, -1),
        Vec4D(0, 0, (2 * farPlane * nearPlane) * nf, 0)
    };
}

Vec2D getProjectedVector(const Vec3D &v, const sf::Image &image, const Vec3D &cameraPos, double camAngleX, double camAngleY) {
    Matrix3x3 combineRotations =  (
        Matrix3x3(Vec3D(1,0,0),Vec3D(0,cos(camAngleX),sin(camAngleX)), Vec3D(0,-sin(camAngleX),cos(camAngleX)))
        *
        Matrix3x3(Vec3D(cos(camAngleY),0,-sin(camAngleY)),Vec3D(0,1,0), Vec3D(sin(camAngleY),0,cos(camAngleY))));

    double nearPlane = 0.001;
    double farPlane = 1000.0;
    Matrix4x4 projectionMatrix = getPerspectiveProjectionMatrix(M_PI / 4, image, nearPlane, farPlane);

    // shifts v to reframe the space as if the camera were at the origin
    Vec3D cameraRelativePosition = v - cameraPos;

    cameraRelativePosition = combineRotations*cameraRelativePosition;
    cameraRelativePosition = Matrix3x3(Vec3D(1,0,0), Vec3D(0,-1,0), Vec3D(0,0,1)) * cameraRelativePosition;
    // extend to 4d
    Vec4D extendedVector(cameraRelativePosition.x, cameraRelativePosition.y, cameraRelativePosition.z, 1.0);

    // apply transformation
    Vec4D transformed = projectionMatrix * extendedVector;

    // ensure no division by zero
    if (std::abs(transformed.w) < 0.0001) {
        transformed.w = 0.0001;
    }

    if (std::abs(transformed.w) < 1e-6) transformed.w = 1e-6;
    double x = transformed.x /transformed.w;
    double y = transformed.y / transformed.w;

    // normalized device coordinates to screen space
    double screenX = (x + 1.0) * 0.5 * image.getSize().x;
    double screenY = (1.0 - y) * 0.5 * image.getSize().y;
    screenX = std::max(0.0, std::min(screenX, static_cast<double>(image.getSize().x - 1)));
    screenY = std::max(0.0, std::min(screenY, static_cast<double>(image.getSize().y - 1)));

    return {screenX, screenY};
}

