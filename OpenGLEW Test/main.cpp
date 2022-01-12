//
//  main.cpp
//  OpenGLEW Test
//
//  Created by Bohdan Andriychuk on 11.01.2022.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "ShaderUtil.hpp"


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

static unsigned int CompileShader( unsigned int type, const std::string& source);

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

static ShaderProgramSource ParseShaderFiles(const std::string VSfilePath, const std::string FSfilePath);

const char* vertexShader = "#version 330 core\n"
"layout (location = 0) in vec4 position;\n"
"void main()\n"
"{\n"
"   gl_Position = position;\n"
"}\0";
const char* fragmentShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0, 0.0, 0.5f);\n"
"}\n\0";

int main(void)
{
    GLFWwindow *window;
    
    if (!glfwInit()) {
        std::cout << "glfw init err" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    std::cout << "I'm apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    
    unsigned int width = 800;
    unsigned int height = 600;
    window = glfwCreateWindow(width, height, "test creen 1", NULL, NULL);
    
    if (!window) {
        std::cout << " can't create window!!!" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    std::cout << "oepngl shader version: " << major << "." << minor << std::endl;
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    if (glewInit() != GLEW_OK) {
        std::cout << "Error ! " << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    float vertices[] = {
        -0.5f, -0.5f,
        0.5f,  -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };
    
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
    

    ShaderProgramSource source = ParseShaderFiles("/Users/b.andriychuk/Projects/OpenGLEW Test/shaders/basicVS.shader", "/Users/b.andriychuk/Projects/OpenGLEW Test/shaders/basicFS.shader");
    std::cout<< "FILE: " << source.FragmentSource << "\n ____ \n" << source.VertexSource << std::endl;
    

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
    
    // Uncommenting this call will result in wireframe polygons.
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
    
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader);
    
    glfwTerminate();
    
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static unsigned int CompileShader( unsigned int type, const std::string& source)
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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

static ShaderProgramSource ParseShaderFiles(const std::string VSfilePath, const std::string FSfilePath) {
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
