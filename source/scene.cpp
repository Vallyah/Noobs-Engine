#include <vector>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"

Scene::Scene(const unsigned int width, const unsigned int height) : scr_width(width), scr_height(height)
{
    /* Create shader programs */
    /*************************/
    _program = std::make_shared<Shader>("../data/Shaders/vertex.glsl",
                                        "../data/Shaders/fragment.glsl");
    _program_shadowmaps = std::make_shared<Shader>("../data/Shaders/vertex_shadowmap.glsl",
                                                   "../data/Shaders/fragment_shadowmap.glsl");
    _program_quad = std::make_shared<Shader>("../data/Shaders/vertex_quad.glsl",
                                             "../data/Shaders/fragment_quad.glsl");
    _program_lightcube = std::make_shared<Shader>("../data/Shaders/vertex.glsl",
                                                  "../data/Shaders/fragment_lightcube.glsl");

    /* Create geometry for plan */
    /****************************/
    std::vector<Vertex> vertices(4);
    vertices[0].Position = glm::vec3(-12.0f, 0.0f, -12.0f);
    vertices[0].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[1].Position = glm::vec3(12.0f, 0.0f, -12.0f);
    vertices[1].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[2].Position = glm::vec3(12.0f, 0.0f, 12.0f);
    vertices[2].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[3].Position = glm::vec3(-12.0f, 0.0f, 12.0f);
    vertices[3].Normal = glm::vec3(0.0f, 1.0f, 0.0f);

    std::vector<unsigned int> indices = { 0, 1, 2,
                                          0, 2, 3 };
    std::vector<Texture> textures = {};

    _mesh = std::make_unique<SimpleMesh>(vertices, indices, textures);

    _planMat = glm::mat4(1.0f); // Identity
    _planMat = glm::translate(_planMat, glm::vec3(0.0f, -2.0f, 0.0f));

    /* Create geometry for light cube */
    /**********************************/
    _pointlight_pos = glm::vec3(0.0f, 0.7f, 4.8f);

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

    _lightcube = std::make_unique<SimpleMesh>(cube_vertices, cube_indices, textures);

    _lightcubeMat = glm::translate(glm::mat4(1.0f), _pointlight_pos);

    /* Create model */
    /****************/
    _model = std::make_unique<Model>("../data/Objects/Cottage/cottage.obj");

    _modelMat = glm::mat4(1.0f); // Identity
    _modelMat = glm::rotate(_modelMat, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    _modelMat = glm::translate(_modelMat, glm::vec3(0.0f, -2.0f, 0.0f));
    _modelMat = glm::scale(_modelMat, glm::vec3(0.1f));

    /* Create screen quad */
    /**********************/

    _screenquad = std::make_unique<ScreenQuad>();

    /* Create camera */
    /*****************/
    _camera = std::make_unique<Camera>(glm::vec3(0.0f, 7.0f, 15.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f), YAW, -30.0f);
    lastX = scr_width / 2.0f;
    lastY = scr_height / 2.0f;
    firstMouse = true;

    /* Create Lights */
    /*****************/
    _dirlight = std::make_unique<DirectionalLight>(glm::vec3(-0.5f, -0.5f, 0.5f),
                                                   glm::vec3(0.1f), glm::vec3(0.4f),
                                                   glm::vec3(7.0f, 7.0f, -7.0f));
}

void Scene::Draw()
{
    // render scene for shadow maps
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, _dirlight->fbo());
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderScene_depthMaps();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // debug with screen quad
    /* glViewport(0, 0, scr_width, scr_height); */
    /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */
    /* _program_quad->bind(); */
    /* _screenquad->Draw(_dirlight->depthMap()); */
    /* _program_quad->unbind(); */

    // render scene normally
    glViewport(0, 0, scr_width, scr_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, _dirlight->depthMap());

    RenderScene_normal();
}

void Scene::RenderScene_normal()
{
    /* Build view + proj matrices */
    /******************************/
    _view = _camera->GetViewMatrix();
    _projection = glm::perspective(_camera->getZoom(), ((float) scr_width) / scr_height,
                                   0.1f, 100.0f);

    /* Bind shader program */
    /***********************/
    _program->bind();

    /* Set camera matrices */
    /***********************/
    _program->setMat4("view", _view);
    _program->setMat4("projection", _projection);

    /* Set lights */
    /**************/
    _program->setVec3("dirLight.direction", _dirlight->direction());
    _program->setVec3("dirLight.ambient", _dirlight->ambient());
    _program->setVec3("dirLight.diffuse", _dirlight->diffuse());
    _program->setMat4("dirlight_spacematrix", _dirlight->lightSpaceMatrix());

    _program->setVec3("pointLight.position", _pointlight_pos);
    _program->setFloat("pointLight.constant", 0.4f);
    _program->setFloat("pointLight.linear", 0.15f);
    _program->setFloat("pointLight.quadratic", 0.0f);
    _program->setVec3("pointLight.ambient", glm::vec3(0.1f));
    _program->setVec3("pointLight.diffuse", glm::vec3(1.0f));

    /* Draw plan */
    /*************/
    _program->setMat4("model", _planMat);
    _program->setVec3("diffuse", glm::vec3(0.3f, 0.6f, 0.2f));
    _program->setInt("hasTexture", 0);

    _mesh->Draw(_program);

    /* Draw model */
    /**************/
    _program->setMat4("model", _modelMat);
    _program->setVec3("diffuse", glm::vec3(0.7f, 0.3f, 0.0f));
    _program->setInt("hasTexture", 1);

    _model->Draw(_program);

    /* Unbind shader program */
    /*************************/
    _program->unbind();

    /* Draw LightCube */
    /******************/
    _program_lightcube->bind();
    _program_lightcube->setMat4("view", _view);
    _program_lightcube->setMat4("projection", _projection);
    _program_lightcube->setMat4("model", _lightcubeMat);

    _lightcube->Draw(_program_lightcube);
    _program_lightcube->unbind();
}

void Scene::RenderScene_depthMaps()
{
    /* Bind shader program */
    /***********************/
    _program_shadowmaps->bind();

    /* Set camera matrices */
    /***********************/
    _program_shadowmaps->setMat4("lightSpaceMatrix", _dirlight->lightSpaceMatrix());

    /* Draw plan */
    /*************/
    _program_shadowmaps->setMat4("model", _planMat);

    _mesh->Draw(_program_shadowmaps);

    /* Draw model */
    /**************/
    _program_shadowmaps->setMat4("model", _modelMat);

    _model->Draw(_program_shadowmaps);

    /* Unbind shader program */
    /*************************/
    _program_shadowmaps->unbind();
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
