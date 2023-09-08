#pragma once

#include <map>

#include "../Shader.h"
#include "../Model.h"

struct Renderer
{
    Shader* shader;
    Model* model;
};
