#ifndef SIMPLEMESH_H
#define SIMPLEMESH_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "shader.h"

/* Largely inspired from learnopengl.com/Model-Loading/Mesh */

struct Texture {
    unsigned int id;
    std::string type;
};

class SimpleMesh {
    public:
        std::vector<glm::vec3> _vertices;
        std::vector<glm::vec3> _normals;
        std::vector<glm::vec2> _uvs;

        std::vector<unsigned int> _indices;
        std::vector<Texture> _textures;

        SimpleMesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
                   std::vector<glm::vec2> uvs, std::vector<unsigned int> indices,
                   std::vector<Texture> textures);
        void Draw(Shader *shader);

    private:
        unsigned int vao, vbo, nbo, uvbo, ebo;
        void setup();
};
#endif
