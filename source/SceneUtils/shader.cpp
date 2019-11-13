/* Inspired from https://github.com/SylvainDeker/Shader-Graph/blob/master/source/preview/Shader.cpp */
#include "shader.h"

#include <fstream> // Reading a file with the C API instead of the C++ API is more efficient
#include <sstream>
#include <iostream>
#include <regex>

#include <glad/glad.h>

/* Shader::Shader(const std::string& vertexshaderpath, const std::string& fragmentshaderpath) */
/* { */
/*     m_fragmentShaderPath = fragmentshaderpath; */
/*     m_vertexShaderPath = vertexshaderpath; */
/*     const ShaderProgramSource shaderProgramSource = parseShaderFile(vertexshaderpath, */
/*                                                                     fragmentshaderpath); */
/*     m_id = createShader(shaderProgramSource.vertex, shaderProgramSource.fragment); */
/*     unbind(); // By default, a shader is unbound after creation. */
/* } */

Shader::Shader(const std::string& vertexshaderpath, const std::string& fragmentshaderpath,
               const std::string& geometryshaderpath)
{
    m_fragmentShaderPath = fragmentshaderpath;
    m_vertexShaderPath = vertexshaderpath;
    m_geometryShaderPath = geometryshaderpath;
    const ShaderProgramSource shaderProgramSource = parseShaderFile(vertexshaderpath,
                                                                    fragmentshaderpath,
                                                                    geometryshaderpath);
    m_id = createShader(shaderProgramSource.vertex, shaderProgramSource.fragment,
                        shaderProgramSource.geometry);
    unbind(); // By default, a shader is unbound after creation.
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::bind() const
{
    glUseProgram(m_id);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

Shader::ShaderProgramSource Shader::parseShaderFile(const std::string& vertexshaderpath,
                                                    const std::string& fragmentshaderpath,
                                                    const std::string& geometryshaderpath)
{
    std::stringstream ss[3];

    // Extract Vertex Shader
    ss[0] << "#version 400 core\n\n"; // Header
    ss[0] << readGLSLFile(vertexshaderpath, 0);

    // Extract Fragment Shader
    ss[1] << "#version 400 core\n\n"; // Header
    ss[1] << readGLSLFile(fragmentshaderpath, 0);

    // Extract Geometry Shader
    if (geometryshaderpath.compare(""))
    {
        ss[2] << "#version 400 core\n\n"; // Header
        ss[2] << readGLSLFile(geometryshaderpath, 0);
    }
    /* else */
    /*     ss[2] << ""; */

    return {ss[0].str(), ss[1].str(), ss[2].str() } ;
}

int Shader::compileShader(unsigned int type, const std::string& source)
{
    int shaderId = glCreateShader(type);
    const char * cSource = source.c_str();

    glShaderSource(shaderId, 1, &cSource, nullptr);
    glCompileShader(shaderId);

    int result = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) /* check if an error occurred */
    {
        int len = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);
        char * errmsg = (char *) alloca(len * sizeof(char));

        glGetShaderInfoLog(shaderId, len, &len, errmsg);

        std::cerr << "Shader::CompileShader : "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << ", compilation failed with" << errmsg << std::endl;
        return -1;
    }
    return shaderId;
}

int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader,
                         const std::string& geometryShader)
{
    int program = glCreateProgram();
    int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    m_fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    if (geometryShader.compare(""))
        m_gs = compileShader(GL_GEOMETRY_SHADER, geometryShader);
    else
        m_gs = 0;

    if (vs < 0 || m_fs < 0 || m_gs < 0) return -1; // The compilation has failed..

    glAttachShader(program, vs);
    glAttachShader(program, m_fs);
    if (geometryShader.compare(""))
        glAttachShader(program, m_gs);

    glLinkProgram(program);

    int valid;
    glGetProgramiv(program, GL_LINK_STATUS, &valid);
    if (!valid)
    {
        std::cerr << "Shader::createShader : program could not be linked" << std::endl;
    }

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(m_fs);
    if (!geometryShader.compare(""))
        glDeleteShader(m_gs);

    return program;
}

void Shader::refreshFragment()
{
    // Read new shader
    std::stringstream new_frag;
    new_frag << "#version 400 core\n\n"; // Header
    new_frag << readGLSLFile(m_fragmentShaderPath, 0);

    new_frag.str();

    // Compile new shader
    int new_fs = compileShader(GL_FRAGMENT_SHADER, new_frag.str());

    if (new_fs == -1)
    {
        std::cerr << "Shader::refresh : could not compile shader" << std::endl;
    }
    else
    {
        // Detach and activate deletion flag of old shader
        glDetachShader(m_id, m_fs);

        // Attach new shader
        glAttachShader(m_id, new_fs);
        m_fs = new_fs;

        glLinkProgram(m_id);

        int valid;
        glGetProgramiv(m_id, GL_LINK_STATUS, &valid);
        if (!valid)
        {
            std::cerr << "Shader::refresh : program could not be linked" << std::endl;
        }

        glValidateProgram(m_id);

        // Set deletion flag of new shader
        glDeleteShader(m_fs);
    }
}

std::string Shader::readGLSLFile(const std::string& filepath, int level)
{
    if (level >= 32)
    {
        std::cerr << "Shader::readGLSLFile : include recursion went too deep" << std::endl;
    }

    std::ifstream f_stream(filepath);

    if (!f_stream.is_open())
    {
        std::cerr << "Shader::readGLSLFile : GLSL file not found : " << filepath << std::endl;
    }


    std::stringstream ss;
    std::string line;

    static const std::regex reg("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
    size_t found = filepath.find_last_of("/");
    std::string folder = filepath.substr(0, found + 1);

    while (std::getline(f_stream, line))
    {
        std::smatch match;
        if (std::regex_search(line, match, reg))
        {
            std::string includepath = folder + match[1].str();
            ss << readGLSLFile(includepath, 0);
        }
        else
            ss << line << "\n";
    }

    return ss.str();
}

int Shader::getUniformLocation(const std::string& name)
{
    const int kUndefinedLocation = -1;
    const int kLocation = glGetUniformLocation(m_id, name.c_str());

    if (kLocation == kUndefinedLocation)
    {
        std::cerr << "Shader::getUniformLocation, uniform : " << name
                  << " not found" << std::endl;
    }

    return kLocation;
}

int Shader::getUniformLocation(const std::string&& name)
{
    const int kUndefinedLocation = -1;
    const int kLocation = glGetUniformLocation(m_id, name.c_str());

    if (kLocation == kUndefinedLocation)
    {
        std::cerr << "Shader::getUniformLocation, uniform : " << name
                  << " not found" << std::endl;
    }

    return kLocation;
}

void Shader::setInt(const std::string& name, int v)
{
    const int kLocation = getUniformLocation(name);
    glUniform1i(kLocation, v);
}

void Shader::setVec3(const std::string& name, const glm::vec3& v)
{
    const int kLocation = getUniformLocation(name);
    glUniform3f(kLocation, v.x, v.y, v.z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& v)
{
    const int kLocation = getUniformLocation(name);
    glUniform4f(kLocation, v.x, v.y, v.z, v.a);
}

void Shader::setFloat(const std::string& name, float v)
{
    const int kLocation = getUniformLocation(name);
    glUniform1f(kLocation, v);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
    const int kLocation = getUniformLocation(name);
    glUniformMatrix4fv(kLocation, 1, GL_FALSE, &mat[0][0]);
}
