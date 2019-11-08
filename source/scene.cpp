#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"

Scene::Scene(const unsigned int width, const unsigned int height) : scr_width(width), scr_height(height)
{
    /* Create shader program */
    /*************************/
    _program = std::make_shared<Shader>("../data/Shaders/vertex.glsl",
                                        "../data/Shaders/fragment.glsl");

    /* Create geometry for plan */
    /****************************/
    std::vector<Vertex> vertices(4);
    vertices[0].Position = glm::vec3(-50.0f, 0.0f, -50.0f);
    vertices[0].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[1].Position = glm::vec3(50.0f, 0.0f, -50.0f);
    vertices[1].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[2].Position = glm::vec3(50.0f, 0.0f, 50.0f);
    vertices[2].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[3].Position = glm::vec3(-50.0f, 0.0f, 50.0f);
    vertices[3].Normal = glm::vec3(0.0f, 1.0f, 0.0f);

    std::vector<unsigned int> indices = { 0, 1, 2,
                                          0, 2, 3 };
    std::vector<Texture> textures = {};

    _mesh = std::make_unique<SimpleMesh>(vertices, indices, textures);

    _planMat = glm::mat4(1.0f); // Identity
    _planMat = glm::translate(_planMat, glm::vec3(0.0f, -2.0f, 0.0f));

    /* Create watch tower model */
    /****************************/
    _towermodel = std::make_unique<Model>("../data/Objects/WoodenTower/wooden watch tower2.obj");

    _towerMat = glm::mat4(1.0f); // Identity
    _towerMat = glm::translate(_towerMat, glm::vec3(0.0f, -2.75f, 0.0f));

    /* Create camera */
    /*****************/
    _camera = std::make_unique<Camera>(glm::vec3(0.0f, 5.0f, 9.0f));
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

    /* Set camera matrices */
    /***********************/
    _program->setMat4("view", _view);
    _program->setMat4("projection", _projection);

    /* Draw plan */
    /*************/
    _program->setMat4("model", _planMat);
    _program->setVec3("diffuse", glm::vec3(0.3f, 0.6f, 0.2f));
    _program->setInt("hasTexture", 0);

    _mesh->Draw(_program);

    /* Draw tower */
    /**************/
    _program->setMat4("model", _towerMat);
    _program->setVec3("diffuse", glm::vec3(0.7f, 0.3f, 0.0f));
    _program->setInt("hasTexture", 1);

    _towermodel->Draw(_program);

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
