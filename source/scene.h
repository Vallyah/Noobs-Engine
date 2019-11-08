#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include <glm/glm.hpp>

#include "SceneUtils/shader.h"
#include "SceneUtils/simpleMesh.h"
#include "SceneUtils/camera.h"
#include "SceneUtils/model.h"

class Scene {
public:
    Scene(const unsigned int width, const unsigned int height);
    ~Scene() {};

    void Draw();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessKeyboard(const char k);
    void ProcessMouseMovement(double xpos, double ypos);
    void ProcessMouseScroll(double xoffset, double yoffset);

private:
    unsigned int scr_width;
    unsigned int scr_height;

    std::shared_ptr<Shader> _program;
    std::shared_ptr<Shader> _program_lightcube;
    std::unique_ptr<SimpleMesh> _mesh;
    std::unique_ptr<SimpleMesh> _lightcube;
    std::unique_ptr<Model> _towermodel;

    glm::vec3 _pointlight_pos;

    std::unique_ptr<Camera> _camera;
    float lastX;
    float lastY;
    bool firstMouse;

    glm::mat4 _planMat;
    glm::mat4 _lightcubeMat;
    glm::mat4 _towerMat;
    glm::mat4 _view;
    glm::mat4 _projection;
};

#endif
