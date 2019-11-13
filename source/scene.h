#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include <glm/glm.hpp>

#include "SceneUtils/shader.h"
#include "SceneUtils/simpleMesh.h"
#include "SceneUtils/camera.h"
#include "SceneUtils/model.h"
#include "SceneUtils/directionalLight.h"
#include "SceneUtils/pointLight.h"
#include "SceneUtils/screenQuad.h"

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
    std::shared_ptr<Shader> _program_shadowmaps;
    std::shared_ptr<Shader> _program_quad;
    std::shared_ptr<Shader> _program_lightcube;

    std::unique_ptr<DirectionalLight> _dirlight;
    std::unique_ptr<PointLight> _pointlight;

    std::unique_ptr<SimpleMesh> _mesh;
    std::unique_ptr<ScreenQuad> _screenquad;
    std::unique_ptr<Model> _model;

    std::unique_ptr<Camera> _camera;
    float lastX;
    float lastY;
    bool firstMouse;

    glm::mat4 _planMat;
    glm::mat4 _modelMat;
    glm::mat4 _view;
    glm::mat4 _projection;

    void RenderScene_normal();
    void RenderScene_depthMaps();
};

#endif
