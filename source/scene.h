#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "SceneUtils/shader.h"
#include "SceneUtils/simpleMesh.h"

class Scene {
public:
    Scene();
    ~Scene() {};

    void Draw();

private:
    std::shared_ptr<Shader> _program;
    std::unique_ptr<SimpleMesh> _mesh;
};

#endif
