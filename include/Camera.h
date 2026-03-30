#pragma once
#include "glm/glm.hpp"

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    // 相机属性
    glm::vec3 mPosition;
    glm::vec3 mFront{0.0f, 0.0f, -1.0f};
    glm::vec3 mUp{0.0f, 1.0f, 0.0f};
    glm::vec3 mRight{1.0f, 0.0f, 0.0f};
    glm::vec3 worldUp;

    // 欧拉角
    float mYaw;
    float mPitch;

    // 相机选项
    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;

    explicit Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW, float pitch = PITCH
    );

    [[nodiscard]] glm::mat4 getViewMatrix() const;

    void processKeyBoard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void processMouseScroll(float yOffset);



private:
    void updateCameraVectors();
};
