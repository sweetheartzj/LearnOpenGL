#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(
    const glm::vec3 position,
    const glm::vec3 up,
    const float yaw, const float pitch
)
    : mPosition(position),
    worldUp(up),
    mYaw(yaw),
    mPitch(pitch),
    mMovementSpeed(SPEED),
    mMouseSensitivity(SENSITIVITY),
    mZoom(ZOOM) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);
    mRight = glm::normalize(glm::cross(mFront, worldUp));
    mUp = worldUp;
}


void Camera::processKeyBoard(const CameraMovement direction, const float deltaTime) {
    const float velocity = mMovementSpeed * deltaTime;
    switch (direction) {
        case CameraMovement::FORWARD:
            mPosition += mFront * velocity;
            break;
        case CameraMovement::BACKWARD:
            mPosition -= mFront * velocity;
            break;
        case CameraMovement::LEFT:
            mPosition -= mRight * velocity;
            break;
        case CameraMovement::RIGHT:
            mPosition += mRight * velocity;
            break;
        default:
            break;
    }
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= mMouseSensitivity;
    yOffset *= mMouseSensitivity;

    mYaw += xOffset;
    mPitch += yOffset;

    if (constrainPitch) {
        if (mPitch > 89.0f) {
            mPitch = 89.0f;
        }
        if (mPitch < -89.0f) {
            mPitch = -89.0f;
        }
    }
    updateCameraVectors();
}

void Camera::processMouseScroll(const float yOffset) {
    mZoom -= yOffset;
    if (mZoom < 1.0f) {
        mZoom = 1.0f;
    }
    if (mZoom > 45.0f) {
        mZoom = 45.0f;
    }
}

