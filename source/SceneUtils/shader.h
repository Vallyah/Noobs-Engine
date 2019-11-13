/* Inspired from https://github.com/SylvainDeker/Shader-Graph/blob/master/source/preview/Shader.h */
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    //Shader(const std::string& vertexshaderpath, const std::string& fragmentshaderpath);
    Shader(const std::string& vertexshaderpath, const std::string& fragmentshaderpath,
           const std::string& geometryshaderpath = "");
    ~Shader();

    /// Bind this Shader (aka program).
    void bind() const;

    /// Unbind this Shader (aka program).
    void unbind() const;

    /// Recompile fragment Shader
    void refreshFragment();

    /// Set a uniform of the type : int.
    void setInt(const std::string &name, int v);

    /// Set a uniform of the type : float.
    void setFloat(const std::string &name, float v);

    /// Set a uniform of the type : vec3.
    void setVec3(const std::string &name, const glm::vec3& v);

    /// Set a uniform of the type : vec4.
    void setVec4(const std::string &name, const glm::vec4& v);

    /// Set a uniform of the type : mat4.
    void setMat4(const std::string &name, const glm::mat4& mat);

    /// Get the id given by openGL.
    const int& id() const { return m_id; }

    /// Get the location of a uniform with its name.
    int getUniformLocation(const std::string& name);

    /// Get the location of a uniform with its name.
    int getUniformLocation(const std::string&& name);

private:
    /// The id given by openGL.
    int m_id;
    std::string m_fragmentShaderPath;
    std::string m_vertexShaderPath;
    std::string m_geometryShaderPath;
    int m_fs;
    int m_gs;

    struct ShaderProgramSource
    {
        std::string vertex;
        std::string fragment;
        std::string geometry;
    };

    /// Extract Vertex shader and Fragment shader codes
    ShaderProgramSource parseShaderFile(const std::string& vertexshaderpath,
                                        const std::string& fragmentshaderpath,
                                        const std::string& geometryshaderpath);

    /// Compile a glsl code depending on its type.
    int compileShader(unsigned int type, const std::string& source);

    /// Create a shader from the vertex code and the fragment code.
    int createShader(const std::string& vertexShader, const std::string& fragmentShader,
                     const std::string& geometryShader);

    /// Read a glsl file and extract code, managing includes recursively
    std::string readGLSLFile(const std::string& filepath, int level);
};

#endif //SHADER_H
