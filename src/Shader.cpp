#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


unsigned int Shader::linkProgram(const unsigned int vertexShader, const unsigned int fragmentShader) {
    int success = 1;
    char infoLog[512];

    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        throw std::runtime_error("Error: shader program linking failed\n" + std::string(infoLog));
    }
    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

Shader::Shader(const unsigned int vertexShader, const unsigned int fragmentShader) {
    mId = linkProgram(vertexShader, fragmentShader);
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    const std::string vertexStr = readFile(vertexPath);
    const std::string fragmentStr = readFile(fragmentPath);
    const char* vertexCode = vertexStr.c_str();
    const char* fragmentCode = fragmentStr.c_str();


    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);

    int success = 1;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        throw std::runtime_error("Error: vertex shader compilation failed\n" + std::string(infoLog));
    }

    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        throw std::runtime_error("Error: fragment shader compilation failed\n" + std::string(infoLog));
    }
    mId = linkProgram(vertexShader, fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(mId);
}

void Shader::use() const {
    glUseProgram(mId);
}

void Shader::setBool(const std::string &name, const bool value) const {
    const int location = glGetUniformLocation(mId, name.c_str());
    glUniform1i(location, value);
}

void Shader::setInt(const std::string &name, const int value) const {
    const int location = glGetUniformLocation(mId, name.c_str());
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, const float value) const {
    const int location = glGetUniformLocation(mId, name.c_str());
    glUniform1f(location, value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    const int location = glGetUniformLocation(mId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}


