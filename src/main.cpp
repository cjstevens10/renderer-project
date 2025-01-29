// main.cpp
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <cmath>
#include "InputHandler.h"
#include <filesystem>





int main() {

    std::cout << "\n\n\n\n" << std::endl;

    std::cout << ("Welcome to my project.\n"
                 "This project renders 3D mesh data stored in .txt files and displays it on your 2D screen. \n"
                 "No external libraries were used for rasterization or lighting computations. All necessary linear algebra was implemented in this project.\n"
                 "User movement is supported:\n\n"
                 "MOVEMENT CONTROLS:\n"
                 "(W KEY: forward)\t"
                 "(S KEY: backward)\t"
                 "(A KEY: left)\t"
                 "(D KEY: right)\n"
                 "(E KEY: down)\t"
                 "(Q KEY: up)\n"
                 "(LEFT ARROW: look left)\t"
                 "(RIGHT ARROW: look right)\t"
                 "(UP ARROW: look up)\t"
                 "(DOWN ARROW: look down)\n\n\n");



    double camAngleX = 0;
    double camAngleY = 0;
    sf::Clock clock;
    Vec3D cameraPos(0, 0, -3);
    Vec3D lightSource(150, 150, -200);
    int screenWidth = 1100;
    int screenHeight = 800;

    std::vector<Mesh> meshes = {loadMeshFromFile(getFileInput())};
    bool lightFollowCamera = lightingPrompt();
    double moveSpeed = getMoveSpeed();
    double lookSpeed = getCamSpeed();
    if (lightFollowCamera) {lightSource = cameraPos;}

    sf::Image image;
    image.create(screenWidth, screenHeight);
    for (unsigned int x = 0; x < screenWidth; ++x) {
        for (unsigned int y = 0; y < screenHeight; ++y) {
            image.setPixel(x, y, sf::Color::Magenta);
        }
    }

    for (const auto& mesh : meshes) {rasterizeMesh(mesh, cameraPos, image, lightSource, camAngleY, camAngleY);}

    sf::Texture texture;
    if (!texture.loadFromImage(image)) {
        std::cerr << "Can't load image" << std::endl;
        return -1;
    }

    sf::Sprite sprite(texture);
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(screenWidth),
                                      static_cast<unsigned int>(screenHeight)),
                        "Rendered Image");


    while (window.isOpen()) {
        if (lightFollowCamera) {lightSource = cameraPos;}

        sf::Time deltaTime = clock.restart();
        double dt = deltaTime.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            // Close the window if the close event is received
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //  check if the camera has moved
        bool cameraChanged = false;

        // left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            cameraPos.x += moveSpeed * dt * std::cos(camAngleY);
            cameraPos.z += moveSpeed * dt * std::sin(camAngleY);
            cameraChanged = true;
        }
        // right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            // Move backward relative to camera's yaw (camAngleY)
            cameraPos.x -= moveSpeed * dt * std::cos(camAngleY);
            cameraPos.z -= moveSpeed * dt * std::sin(camAngleY);
            cameraChanged = true;
        }
        // forward
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            // Move left relative to camera's yaw (camAngleY)
            cameraPos.x -= moveSpeed * dt * sin(camAngleY);
            cameraPos.z += moveSpeed * dt * cos(camAngleY);
            cameraChanged = true;
        }
        // backward
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            // Move right relative to camera's yaw (camAngleY)
            cameraPos.x += moveSpeed * dt * sin(camAngleY);
            cameraPos.z -= moveSpeed * dt * cos(camAngleY);
            cameraChanged = true;
        }
        // down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            cameraPos.y -= moveSpeed * dt;
            cameraChanged = true;
        }
        // up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            cameraPos.y += moveSpeed * dt;
            cameraChanged = true;
        }
        // look down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            camAngleX -= lookSpeed * dt;
            // Clamp camAngleX to prevent flipping (optional)
            camAngleX = std::clamp(camAngleX, -M_PI / 2.0 + 0.01, M_PI / 2.0 - 0.01);
            cameraChanged = true;
        }
        // look up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            camAngleX += lookSpeed * dt;
            // Clamp camAngleX to prevent flipping (optional)
            camAngleX = std::clamp(camAngleX, -M_PI / 2.0 + 0.01, M_PI / 2.0 - 0.01);
            cameraChanged = true;
        }
        // look left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            camAngleY -= lookSpeed * dt;
            cameraChanged = true;
        }
        // look right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            camAngleY += lookSpeed * dt;
            cameraChanged = true;
        }
        // re-rasterize mesh and refresh the screen if camera has moved
        if (cameraChanged) {
            for (const auto& mesh : meshes) {rasterizeMesh(mesh, cameraPos, image, lightSource, camAngleX, camAngleY);}

            // update texture with newly-drawn image
            if (!texture.loadFromImage(image)) {
                std::cerr << "failed to load texture from image\n";
            }
            sprite.setTexture(texture, true);
        }

        // refresh window
        for (int i = 0; i < screenWidth; ++i) {
            for (int j = 0; j < screenHeight; ++j) {
                image.setPixel(i, j, sf::Color::Magenta);
            }
        }
        
        window.draw(sprite);
        window.display();

    }

    return 0;
}
