#include <vector>

#include "scene.h"

Scene::Scene()
{
    /* Create shader program */
    /*************************/
    _program = std::make_shared<Shader>("../data/Shaders/vertex.glsl",
                                   "../data/Shaders/fragment.glsl");

    /* Create geometry */
    /*******************/
    std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, -0.5f, 0.0f), // left
        glm::vec3(0.5f, -0.5f, 0.0f), // right
        glm::vec3(0.0f,  0.5f, 0.0f)  // top
    };

    std::vector<glm::vec3> normals = {
       glm::vec3(0.0f, 0.0f, -1.0f),
       glm::vec3(0.0f, 0.0f, -1.0f),
       glm::vec3(0.0f, 0.0f, -1.0f)
    };

    std::vector<glm::vec2> uvs = {};

    std::vector<unsigned int> indices = { 0, 1, 2 };
    std::vector<Texture> textures = {};

    _mesh = std::make_unique<SimpleMesh>(vertices, normals, uvs, indices, textures);
}

void Scene::Draw()
{
    /* Draw */
    /********/
    _program->bind();
    _program->setVec3("diffuse", glm::vec3(0.1, 1, 0.1));
    _mesh->Draw(_program);

    _program->unbind();
    
    return;
}
