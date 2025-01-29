//
// Created by Cooper Stevens on 12/2/24.
//

#ifndef IOHANDLER_H
#define IOHANDLER_H
#include "LinAlg.h"
#include "Rasterizer.h"

double getMoveSpeed();

double getCamSpeed();

bool lightingPrompt();

std::string getFileInput();

Mesh loadMeshFromFile(const std::string& filename);
#endif





