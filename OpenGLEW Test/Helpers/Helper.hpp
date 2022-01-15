//
//  Helper.hpp
//  OpenGLEW Test
//
//  Created by Bohdan Andriychuk on 15.01.2022.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tgmath.h>

#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifdef DEBUG
    #define GLCallV( x ) \
        GLClearErrors(); \
        x; \
        GLCheckErrors( #x, __FILE__, __LINE__);
    #define GLCall( x ) [&]() { \
        GLClearErrors(); \
        auto retVal = x; \
        GLCheckErrors( #x, __FILE__, __LINE__); \
        return retVal; \
        }()
#else
    #define GLCallV( x ) x
    #define GLCall( x ) x
#endif


void GLClearErrors();

void GLCheckErrors(const char* function, const char* file, int line);
