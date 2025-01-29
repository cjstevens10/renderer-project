//
// Created by Cooper Stevens on 12/5/24.
//

#include "InputHandler.h"
#include <fstream>
#include <iostream>
#include <sstream>
namespace fs = std::filesystem;

double getMoveSpeed() {
    bool firstLoop = true;
    while (true) {
        if (firstLoop) std::cout << "Enter your desired camera movement speed as a decimal number (1 is recommended)\n";
        firstLoop = false;
        double speed;
        std::cin >> speed;
        if (speed > 0) return speed;
        std::cerr << "Movement speed must be a decimal value greater than 0. Try again:\n";
    }
}

double getCamSpeed() {
    bool firstLoop = true;
    while (true) {
        if (firstLoop) std::cout << "Enter your desired camera turning speed as a decimal number (1 is recommended)\n";
        firstLoop = false;
        double speed;
        std::cin >> speed;
        if (speed > 0) return speed;
        std::cerr << "Camera turning speed must be a decimal value greater than 0. Try again:\n";
    }
}

//  get camera position from the user
bool lightingPrompt() {
    std::cout << "Would you like the point light to follow the camera? It will be stationary otherwise and could make the mesh difficult to see. (enter y if yes, anything else if no)\n";
    char response;
    std::cin >> response;
    return (response == 'y');
}

std::string getFileInput() {

    std::string path = "../inputs";
    std::vector<std::string> files;


    // Iterate over the files in the directory
    for (const auto &entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }

    // List all files in the directory
    std::cout << "AVAILABLE FILES:\n";
    for (const auto &file : files) {
        std::cout << " - " << file << "\t";
    }
    std::cout << "\n";

    // Prompt the user to choose a file
    bool firstLoop = true;
    while (true) {
        if (firstLoop) {std::cout << "Please enter the full name filename of the object you want to render:\n";}
        firstLoop = false;
        std::string filename;
        std::cin >> filename;

        // Ensure the file is valid
        if (std::find(files.begin(), files.end(), filename) != files.end()) {
            return path + "/" + filename;

        }
        std::cerr << "Invalid file name. Try again:\n";


    }

}


Mesh loadMeshFromFile(const std::string& filename) {
    std::vector<Triangle3D> triangles;

    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "can't open file '" << filename << "'." << std::endl;
        return Mesh(triangles);
    }

    std::string line;
    int lineNumber = 0;
    bool apply =false;

    Vec3D t = Vec3D(0,0,0);
    while (std::getline(infile, line)) {
        ++lineNumber;
        // remove whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        std::istringstream l(line);

        if (line[0] == '!') {apply = true; continue;}

        // skip empty lines
        if (line.empty() || line[0] == '#') continue;


        double x1, y1, z1;
        double x2, y2, z2;
        double x3, y3, z3;
        if (!(l >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3)) {
            std::cerr << "(debug) invalid triangle format at line " << lineNumber << ": " << line << std::endl;
            continue;
        }
        Vec3D a(x1, y1, z1);
        Vec3D b(x2, y2, z2);
        Vec3D c(x3, y3, z3);

        triangles.push_back(Triangle3D(a,b,c));
    }
    infile.close();
    Mesh mesh(triangles);

    if (apply) {ensureNormalsFaceOutward(mesh);}
    if (filename == "../inputs/remy.txt") mesh.translate(Vec3D(0,-200,300));


    return mesh;
}


