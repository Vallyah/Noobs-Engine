#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <memory>
#include <glm/glm.hpp>

#include "simpleMesh.h"
#include "shader.h"

class PointLight {
public:
    PointLight(glm::vec3 position, glm::vec3 ambient = glm::vec3(0.1f),
               glm::vec3 diffuse = glm::vec3(1.0f), float constant = 0.4f,
               float linear = 0.15f, float quadratic = 0.0f);
    ~PointLight() {};

    void DrawLightCube(std::shared_ptr<Shader> shader);

    inline glm::vec3 position() { return _position; };
    inline glm::vec3 ambient() { return _ambient; };
    inline glm::vec3 diffuse() { return _diffuse; };
    inline float constant() { return _constant; };
    inline float linear() { return _linear; };
    inline float quadratic() { return _quadratic; };

private:
    glm::vec3 _position;
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    float _constant;
    float _linear;
    float _quadratic;

    unsigned int _depthCubeMap;
    unsigned int _depthMapFBO;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    std::unique_ptr<SimpleMesh> _lightcube;
    glm::mat4 _lightcubeMat;
};

#endif
