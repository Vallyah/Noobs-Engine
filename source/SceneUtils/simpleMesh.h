#ifndef SIMPLEMESH_H
#define SIMPLEMESH_H

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "shader.h"

/* Largely inspired from learnopengl.com/Model-Loading/Mesh */

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class SimpleMesh {
public:
    SimpleMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
               std::vector<Texture> textures);
    ~SimpleMesh() {}
    void Draw(std::shared_ptr<Shader> shader);

private:
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;

    unsigned int vao, vbo, ebo;
    void setup();
};
#endif
