#include "camera.h"
#include <cmath>

Camera::Camera() {
    position[0] = 0.0f;
    position[1] = 50.0f;
    position[2] = 100.0f;
    orientation[0] = 0.0f;
    orientation[1] = -15.0f;
}

void Camera::moveForward(float distance) {
    position[0] += distance * std::sin(orientation[0] * 3.14159f / 180.0f);
    position[2] -= distance * std::cos(orientation[0] * 3.14159f / 180.0f);
}

void Camera::moveBackward(float distance) {
    moveForward(-distance);
}

void Camera::moveLeft(float distance) {
    position[0] -= distance * std::cos(orientation[0] * 3.14159f / 180.0f);
    position[2] -= distance * std::sin(orientation[0] * 3.14159f / 180.0f);
}

void Camera::moveRight(float distance) {
    moveLeft(-distance);
}

void Camera::lookAround(float horizontalAngle, float verticalAngle) {
    orientation[0] += horizontalAngle;
    orientation[1] += verticalAngle;

    if (orientation[1] > 89.0f) orientation[1] = 89.0f;
    if (orientation[1] < -89.0f) orientation[1] = -89.0f;
}