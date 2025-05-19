#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ModelShader {
public:
    // Constructor to load and compile shaders
    ModelShader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexCode = ParseShader(vertexPath);
        std::string fragmentCode = ParseShader(fragmentPath);

        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

        LinkShaders(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // Destructor to clean up shader program
    ~ModelShader() {
        glDeleteProgram(ID);
    }

    // Bind the shader program
    void Bind() const {
        glUseProgram(ID);
    }

    // Unbind the shader program
    void Unbind() const {
        glUseProgram(0);
    }

    // Set uniform mat4
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const {
        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location == -1) {
            std::cerr << "Uniform " << name << " not found!" << std::endl;
            return;
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    // Set uniform vec3
    void SetUniform3f(const std::string& name, float v0, float v1, float v2) const {
        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location == -1) {
            std::cerr << "Uniform " << name << " not found!" << std::endl;
            return;
        }
        glUniform3f(location, v0, v1, v2);
    }

    void setVec3(const std::string& name, const glm::vec3& value) const {
        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location == -1) {
            std::cerr << "Uniform " << name << " not found!" << std::endl;
            return;
        }
        glUniform3f(location, value.x, value.y, value.z);
    }

    void setFloat(const std::string& name, float value) const {
        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location == -1) {
            std::cerr << "Uniform " << name << " not found!" << std::endl;
            return;
        }
        glUniform1f(location, value);
    }

    GLuint GetProgramID() const { return ID; }
   
private:
    
    GLuint ID;
    // Read shader source code from file
    std::string ParseShader(const std::string& filePath) {
        std::ifstream file(filePath);
        std::stringstream stream;

        if (file.is_open()) {
            stream << file.rdbuf();
            file.close();
        }
        else {
            std::cerr << "Failed to open shader file: " << filePath << std::endl;
        }

        return stream.str();
    }

    // Compile a shader of a given type
    GLuint CompileShader(GLenum type, const std::string& source) {
        GLuint shaderID = glCreateShader(type);
        const char* sourceCStr = source.c_str();
        glShaderSource(shaderID, 1, &sourceCStr, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logSize;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
            std::string log(logSize, '\0');
            glGetShaderInfoLog(shaderID, logSize, &logSize, &log[0]);
            std::cerr << "shader compilation failed:\n" << log << std::endl;
        }

        return shaderID;
    }

    // Link vertex and fragment shaders into a shader program
    void LinkShaders(GLuint vertexShader, GLuint fragmentShader) {
        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);

        GLint success;
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logSize;
            glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logSize);
            std::string log(logSize, '\0');
            glGetProgramInfoLog(ID, logSize, &logSize, &log[0]);
            std::cerr << "Program linking failed:\n" << log << std::endl;
        }
    }
};

#endif // SHADER_H
