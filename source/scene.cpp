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

    _camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 1.0f));
    lastX = scr_width / 2.0f;
    lastY = scr_height / 2.0f;
    firstMouse = true;

    _model = glm::mat4(1.0f); // Identity
}

void Scene::Draw()
{
    /* Draw */
    /********/

    _view = _camera->GetViewMatrix();
    _projection = glm::perspective((_camera->getZoom()),
                ((float) scr_width) / scr_height,
                0.1f, 100.0f);

    _program->bind();

    _program->setMat4("model", _model);
    _program->setMat4("view", _view);
    _program->setMat4("projection", _projection);

    _program->setVec3("diffuse", glm::vec3(0.1, 1, 0.1));

    _mesh->Draw(_program);

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
