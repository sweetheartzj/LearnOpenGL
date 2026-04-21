#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"

constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

auto lastX = WIDTH / 2.0f;
auto lastY = HEIGHT / 2.0f;
auto firstMouse = true;

glm::vec3 lightPos{1.2f, 0.0f, 2.0f};

Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyBoard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyBoard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyBoard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyBoard(RIGHT, deltaTime);
    }


}

void mouseCallback(GLFWwindow* window, const double xPosIn, const double yPosIn) {
    const auto xPos = static_cast<float>(xPosIn);
    const auto yPos = static_cast<float>(yPosIn);

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    // glfw y 坐标是反的，所以要反过来
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    constexpr  float sensitivity = 0.05f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, const double xOffset, const double yOffset) {
    camera.processMouseScroll(static_cast<float>(yOffset));
}

void framebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
    std::cout << "Framebuffer size callback: " << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);

    // 定义顶点数据
    constexpr float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

    const Shader lightingShader{"./shaders/light/vert.glsl", "./shaders/light/frag.glsl"};
    const Shader lampShader{"./shaders/light/lamp.vert.glsl", "./shaders/light/lamp.frag.glsl"};


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int lightingVAO;
    glGenVertexArrays(1, &lightingVAO);
    glBindVertexArray(lightingVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    unsigned int lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    const unsigned int diffuseMap = Shader::loadTexture("./resources/maps/diffuse_map.png");
    const unsigned int specularMap = Shader::loadTexture("./resources/maps/specular_map.png");

    // 设置线框模式
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window)) {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto model = glm::mat4(1.0f);
        auto view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.mZoom), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

        glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f);
        glm::vec3 ambientColor = lightColor * glm::vec3(0.1f);

        lightingShader.use();
        // lightingShader.setMat4("model", glm::rotate(model, glm::radians(20.0f * currentFrame), glm::vec3{0.0f, 1.0f, 0.0f}));
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("viewPos", camera.mPosition);
        lightingShader.setTexture("material.diffuse", 0, diffuseMap);
        lightingShader.setTexture("material.specular", 1, specularMap);
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setVec3("light.position", camera.mPosition);
        lightingShader.setVec3("light.direction", camera.mFront);
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

        lightingShader.setVec3("light.ambient", ambientColor);
        lightingShader.setVec3("light.diffuse", diffuseColor);
        lightingShader.setVec3("light.specular", {1.0f, 1.0f, 1.0f});

        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.09f);
        lightingShader.setFloat("light.quadratic", 0.032f);

        glBindVertexArray(lightingVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        for (auto cubePosition : cubePositions) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePosition);
            model = glm::rotate(model, glm::radians(20.0f * currentFrame), glm::vec3{1.0f, 0.3f, 0.5f});
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // lampShader.use();
        // model = glm::mat4(1.0f);
        // model = glm::translate(model, lightPos);
        // model = glm::scale(model, glm::vec3{0.2f});
        // lampShader.setMat4("model", model);
        // lampShader.setMat4("view", view);
        // lampShader.setMat4("projection", projection);
        // lampShader.setVec3("lightColor", lightColor);
        //
        // glBindVertexArray(lampVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);


        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // 释放所有资源
    glDeleteVertexArrays(1, &lightingVAO);
    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}