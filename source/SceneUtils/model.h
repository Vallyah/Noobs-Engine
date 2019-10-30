#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <memory>

#include "simpleMesh.h"
#include "shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/* Largely inspired from learnopengl.com/Model-Loading/Model */

class Model
{
    public:
        Model(char *path, bool gamma = false) : gammaCorrection(gamma)
        {
            loadModel(path);
        }
        void Draw(std::shared_ptr<Shader> shader);	

    private:
        std::vector<Texture> loaded_textures;
        std::vector<SimpleMesh> meshes;
        std::string directory;
        bool gammaCorrection;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        SimpleMesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                              aiTextureType type, std::string typeName);
};

#endif
