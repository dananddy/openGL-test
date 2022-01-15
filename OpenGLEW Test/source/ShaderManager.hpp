//
//  SahderManager.hpp
//  OpenGLEW Test
//
//  Created by Bohdan Andriychuk on 14.01.2022.
//

#include <GL/glew.h>
#include <iostream>
#include <fstream>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    unsigned int CompileShader( unsigned int type, const std::string& source);
    void CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    ShaderProgramSource ParseShaderFiles(const std::string VSfilePath, const std::string FSfilePath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

