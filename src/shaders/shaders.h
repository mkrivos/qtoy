///
/// \package qtoy
///
/// \author Marian Krivos <marian.krivos@rsys.sk>
/// \date 9. 7. 2023 
/// \brief definicia typu
///
/// (C) Copyright 2023 R-SYS s.r.o
/// All rights reserved.
///


#pragma once

#include <glm/ext/vector_int3.hpp>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

struct ShaderParameters
{
    glm::vec4* image;
    glm::vec2 resolution;
    uint8_t* serif;
    glm::ivec2 serifSize;

    glm::vec3 params;
    int function = 0;
    float gamma = 2.2;
    float weight = 1;
    int fontSize = 12;

    bool parallelRun {true};
};

extern void computeFrame(const ShaderParameters& params);



