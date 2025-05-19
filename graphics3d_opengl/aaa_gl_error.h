#ifndef GLERROR_H
#define GLERROR_H


//#include "GLFW/glfw3.h"
#include <iostream>

#define GLCheckError() GLError::CheckError(__FILE__, __LINE__)

class GLError {
public:
    static void CheckError(const char* file, int line) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << " in " << file << " at line " << line << std::endl;
        }
    }
};

#endif // GLERROR_H
