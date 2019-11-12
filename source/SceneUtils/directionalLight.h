#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <glm/glm.hpp>

class DirectionalLight {
public:
    // position is used for shadow mapping
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 position);
    ~DirectionalLight() {};

    inline glm::vec3 direction() { return _direction; };
    inline glm::vec3 ambient() { return _ambient; };
    inline glm::vec3 diffuse() { return _diffuse; };
    inline glm::vec3 position() { return _position; };

private:
    glm::vec3 _direction;
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _position;

    unsigned int depthMapFBO;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMap;
};

#endif
