//
//  Helper.cpp
//  OpenGLEW Test
//
//  Created by Bohdan Andriychuk on 15.01.2022.
//

#include "Helper.hpp"

void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

void GLCheckErrors(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OPENGL ERROR: " << error << "\nfunction:" << function << std::endl << "file:" << file << "\nline -> " << line << "]\n" << std::endl;
    }
}
