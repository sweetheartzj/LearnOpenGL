#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>


class Shader {
public:
    unsigned int mId;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    Shader(unsigned int vertexShader, unsigned int fragmentShader);

    ~Shader();

    void use() const;

    // uniform 函数
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec) const;

    static unsigned int loadTexture(const std::string& path);
    void setTexture(const std::string& name, unsigned int textureIndex, unsigned int textureId) const;

private:
    static std::string readFile(const std::string& path) {
        const std::ifstream file(path);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
};
