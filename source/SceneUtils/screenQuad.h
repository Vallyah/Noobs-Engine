#ifndef SCREENQUAD_H
#define SCREENQUAD_H

#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "shader.h"

class ScreenQuad {
public:
    ScreenQuad();
    ~ScreenQuad() {}
    void Draw(unsigned int texture);

private:
    unsigned int vao, vbo;
};
#endif
