#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"

Scene::Scene(const unsigned int width, const unsigned int height) : scr_width(width), scr_height(height)
{
    /* Create shader programs */
    /*************************/
    _program = std::make_shared<Shader>("../data/Shaders/vertex.glsl",
                                        "../data/Shaders/fragment.glsl",
                                        "",
                                        "../data/Shaders/tess_control.glsl",
                                        "../data/Shaders/tess_evaluation.glsl");

    /* Create geometry for polyhedron */
    /**********************************/
    std::vector<Vertex> vertices(6);
    vertices[0].Position = glm::vec3(1.0f, 0.0f, 0.0f);
    vertices[0].Normal = vertices[0].Position;
    vertices[1].Position = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[1].Normal = vertices[1].Position;
    vertices[2].Position = glm::vec3(0.0f, 0.0f, 1.0f);
    vertices[2].Normal = vertices[2].Position;
    vertices[3].Position = glm::vec3(-1.0f, 0.0f, 0.0f);
    vertices[3].Normal = vertices[3].Position;
    vertices[4].Position = glm::vec3(0.0f, -1.0f, 0.0f);
    vertices[4].Normal = vertices[4].Position;
    vertices[5].Position = glm::vec3(0.0f, 0.0f, -1.0f);
    vertices[5].Normal = vertices[5].Position;

    std::vector<unsigned int> indices = { 2, 0, 1,
                                          2, 0, 4,
                                          5, 0, 4,
                                          5, 0, 1,
                                          5, 3, 1,
                                          5, 3, 4,
                                          2, 3, 4,
                                          2, 3, 1 };

    std::vector<Texture> textures = {};

    _mesh = std::make_unique<SimpleMesh>(vertices, indices, textures);

    _meshMat = glm::mat4(1.0f); // Identity

    lod = 0.0f;

    /* Create camera */
    /*****************/
    _camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 6.0f));
    lastX = scr_width / 2.0f;
    lastY = scr_height / 2.0f;
    firstMouse = true;
}

void Scene::Draw()
{
    /* Build view + proj matrices */
    /******************************/
    _view = _camera->GetViewMatrix();
    _projection = glm::perspective((_camera->getZoom()),
                ((float) scr_width) / scr_height,
                0.1f, 100.0f);

    /* Bind shader program */
    /***********************/
    _program->bind();

    glPatchParameteri(GL_PATCH_VERTICES, 3); // tesselation patch size

    /* Set camera matrices */
    /***********************/
    _program->setMat4("view", _view);
    _program->setMat4("projection", _projection);
    _program->setVec3("viewPos", _camera->getPosition());

    /* Set lights */
    /**************/
    _program->setVec3("dirLight.direction", glm::vec3(0.0f, 0.0f, -1.0f));
    _program->setVec3("dirLight.ambient", glm::vec3(0.8f));
    _program->setVec3("dirLight.diffuse", glm::vec3(0.8f));
    _program->setVec3("dirLight.specular", glm::vec3(1.0f));

    /* Draw polyhedron */
    /*******************/
    _program->setFloat("lod", lod);
    _program->setMat4("model", _meshMat);
    _program->setVec3("ambient", glm::vec3(0.02f, 0.17f, 0.02f));
    _program->setVec3("diffuse", glm::vec3(0.07f, 0.61f, 0.07f));
    _program->setVec3("specular", glm::vec3(0.63f, 0.72f, 0.63f));
    _program->setFloat("shininess", 128.0f * 0.6f);

    _mesh->Draw(_program);

    /* Unbind shader program */
    /*************************/
    _program->unbind();
}

void Scene::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    _camera->ProcessKeyboard(direction, deltaTime);
}

void Scene::ProcessKeyboard(const char k)
{
    switch (k)
    {
        case 'r':
            _program->refreshFragment();
            break;
        case '0':
            lod = 0;
            break;
        case '1':
            lod = 1;
            break;
        case '2':
            lod = 2;
            break;
        case '3':
            lod = 3;
            break;
        case '4':
            lod = 4;
            break;
        case '5':
            lod = 5;
            break;
        default:
            break;
    }
}

void Scene::ProcessMouseMovement(double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    _camera->ProcessMouseMovement(xoffset, yoffset);
}

void Scene::ProcessMouseScroll(double xoffset, double yoffset)
{
    _camera->ProcessMouseScroll(yoffset);
}
