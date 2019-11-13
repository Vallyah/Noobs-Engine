#include "pointLight.h"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
                       float constant, float linear, float quadratic) : _position(position),
                       _ambient(ambient), _diffuse(diffuse), _constant(constant),
                       _linear(linear), _quadratic(quadratic)
{
    /* Create geometry for light cube */
    /**********************************/
    std::vector<Vertex> cube_vertices(8);
    cube_vertices[0].Position = glm::vec3(0.1f);
    cube_vertices[1].Position = glm::vec3(0.1f, 0.1f, -0.1f);
    cube_vertices[2].Position = glm::vec3(0.1f, -0.1f, 0.1f);
    cube_vertices[3].Position = glm::vec3(0.1f, -0.1f, -0.1f);
    cube_vertices[4].Position = glm::vec3(-0.1f, 0.1f, 0.1f);
    cube_vertices[5].Position = glm::vec3(-0.1f, 0.1f, -0.1f);
    cube_vertices[6].Position = glm::vec3(-0.1f, -0.1f, 0.1f);
    cube_vertices[7].Position = glm::vec3(-0.1f);

    std::vector<unsigned int> cube_indices = { 0, 1, 3,
                                               0, 3, 2,
                                               0, 5, 1,
                                               0, 4, 5,
                                               0, 6, 4,
                                               0, 2, 6,
                                               7, 3, 1,
                                               7, 1, 5,
                                               7, 5, 4,
                                               7, 4, 6,
                                               7, 2, 3,
                                               7, 6, 2 };

    std::vector<Texture> textures = {};

    _lightcube = std::make_unique<SimpleMesh>(cube_vertices, cube_indices, textures);

    _lightcubeMat = glm::translate(glm::mat4(1.0f), _position);
}

void PointLight::DrawLightCube(std::shared_ptr<Shader> shader)
{
    shader->setMat4("model", _lightcubeMat);
    _lightcube->Draw(shader);
}
