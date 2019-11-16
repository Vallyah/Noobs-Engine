#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"

Scene::Scene(const unsigned int width, const unsigned int height) : scr_width(width), scr_height(height)
{
    /* Create shader programs */
    /*************************/
    _pntriangles_program = std::make_shared<Shader>("../data/Shaders/vertex.glsl",
                                        "../data/Shaders/fragment.glsl",
                                        "",
                                        "../data/Shaders/pntriangles_control.glsl",
                                        "../data/Shaders/pntriangles_evaluation.glsl");
    _phong_program = std::make_shared<Shader>("../data/Shaders/vertex.glsl",
                                        "../data/Shaders/fragment.glsl",
                                        "",
                                        "../data/Shaders/phong_control.glsl",
                                        "../data/Shaders/phong_evaluation.glsl");

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

    _pntriangles_mat = glm::mat4(1.0f); // Identity
    _pntriangles_mat = glm::translate(_pntriangles_mat, glm::vec3(-2.0f, 0.0f, 0.0f));
    _phong_mat = glm::mat4(1.0f); // Identity
    _phong_mat = glm::translate(_phong_mat, glm::vec3(2.0f, 0.0f, 0.0f));

    lod = 0.0f;
    glPatchParameteri(GL_PATCH_VERTICES, 3); // tesselation patch size

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
    _pntriangles_program->bind();

    /* Set camera matrices */
    /***********************/
    _pntriangles_program->setMat4("view", _view);
    _pntriangles_program->setMat4("projection", _projection);
    _pntriangles_program->setVec3("viewPos", _camera->getPosition());

    /* Set lights */
    /**************/
    _pntriangles_program->setVec3("dirLight.direction", glm::vec3(0.0f, 0.0f, -1.0f));
    _pntriangles_program->setVec3("dirLight.ambient", glm::vec3(0.8f));
    _pntriangles_program->setVec3("dirLight.diffuse", glm::vec3(0.8f));
    _pntriangles_program->setVec3("dirLight.specular", glm::vec3(1.0f));

    /* Draw polyhedron */
    /*******************/
    _pntriangles_program->setFloat("lod", lod);
    _pntriangles_program->setMat4("model", _pntriangles_mat);
    _pntriangles_program->setVec3("ambient", glm::vec3(0.02f, 0.17f, 0.02f));
    _pntriangles_program->setVec3("diffuse", glm::vec3(0.07f, 0.61f, 0.07f));
    _pntriangles_program->setVec3("specular", glm::vec3(0.63f, 0.72f, 0.63f));
    _pntriangles_program->setFloat("shininess", 128.0f * 0.6f);

    _mesh->Draw(_pntriangles_program);

    /* Unbind shader program */
    /*************************/
    _pntriangles_program->unbind();

    /* Bind shader program */
    /***********************/
    _phong_program->bind();

    /* Set camera matrices */
    /***********************/
    _phong_program->setMat4("view", _view);
    _phong_program->setMat4("projection", _projection);
    _phong_program->setVec3("viewPos", _camera->getPosition());

    /* Set lights */
    /**************/
    _phong_program->setVec3("dirLight.direction", glm::vec3(0.0f, 0.0f, -1.0f));
    _phong_program->setVec3("dirLight.ambient", glm::vec3(0.8f));
    _phong_program->setVec3("dirLight.diffuse", glm::vec3(0.8f));
    _phong_program->setVec3("dirLight.specular", glm::vec3(1.0f));

    /* Draw polyhedron */
    /*******************/
    _phong_program->setFloat("lod", lod);
    _phong_program->setMat4("model", _phong_mat);
    _phong_program->setVec3("ambient", glm::vec3(0.02f, 0.17f, 0.02f));
    _phong_program->setVec3("diffuse", glm::vec3(0.07f, 0.61f, 0.07f));
    _phong_program->setVec3("specular", glm::vec3(0.63f, 0.72f, 0.63f));
    _phong_program->setFloat("shininess", 128.0f * 0.6f);

    _mesh->Draw(_phong_program);

    /* Unbind shader program */
    /*************************/
    _phong_program->unbind();

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
            _pntriangles_program->refreshFragment();
            _phong_program->refreshFragment();
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
