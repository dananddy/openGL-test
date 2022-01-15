//
//  SahderManager.cpp
//  OpenGLEW Test
//
//  Created by Bohdan Andriychuk on 14.01.2022.
//

#include "ShaderManager.hpp"

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

unsigned int Shader::CompileShader( unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* mesaege = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, mesaege);
        
        std::cout << "Failed to copile "<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << mesaege << std::endl;
        
        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

void Shader::CreateShader(const std::string& VSfilePath, const std::string& FSfilePath)
{
    ShaderProgramSource source = ParseShaderFiles("/Users/b.andriychuk/Projects/OpenGLEW Test/shaders/basicVS.shader", "/Users/b.andriychuk/Projects/OpenGLEW Test/shaders/basicFS.shader");
    //std::cout<< "FILE: " << source.FragmentSource << "\n ____ \n" << source.VertexSource << std::endl;
    
    unsigned int program = glCreateProgram();
    unsigned int vs = Shader::CompileShader(GL_VERTEX_SHADER, source.VertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    ID = program;
}

ShaderProgramSource Shader::ParseShaderFiles(const std::string VSfilePath, const std::string FSfilePath)
{
    std::ifstream VSfile(VSfilePath);
    std::string str_vs;
    std::string VScontent;
    while (std::getline(VSfile, str_vs)) {
        VScontent.append(str_vs + "\n");
    }
    
    std::ifstream FSfile(FSfilePath);
    std::string str_fs;
    std::string FScontent;
    while (std::getline(FSfile, str_fs)) {
        FScontent.append(str_fs + "\n");
    }
    
    return
    {
        VScontent, FScontent
    };
}
