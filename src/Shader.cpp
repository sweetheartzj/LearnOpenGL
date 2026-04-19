#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



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
        std::cout << "Error: shader program linking failed\n" << infoLog << std::endl;
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
        std::cout << "Error: vertex shader compilation failed\n" << infoLog << std::endl;
        throw std::runtime_error("Error: vertex shader compilation failed\n" + std::string(infoLog));
    }

    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "Error: fragment shader compilation failed\n" << infoLog << std::endl;
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

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const {
    const int location = glGetUniformLocation(mId, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

unsigned int Shader::loadTexture(const std::string &path) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        } else {
            throw std::runtime_error("Error: unsupported number of channels: " + std::to_string(nrChannels));
        }

        unsigned int textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
        return textureId;
    } else {
        std::cout << "Failed to load texture path: " << path << std::endl;
        stbi_image_free(data);
        throw std::runtime_error("Failed to load texture path: " + path);
    }
}

void Shader::setTexture(const std::string &name, const unsigned int textureIndex, const unsigned int textureId) const {
    setInt(name, static_cast<int>(textureIndex));
    const unsigned int textureUnit = GL_TEXTURE0 + textureIndex;
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureId);
}


