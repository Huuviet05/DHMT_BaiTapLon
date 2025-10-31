#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    Camera();
    void moveForward(float distance);
    void moveBackward(float distance);
    void moveLeft(float distance);
    void moveRight(float distance);
    void lookAround(float horizontalAngle, float verticalAngle);

private:
    float position[3]; // x, y, z coordinates
    float orientation[2]; // horizontal and vertical angles
};

#endif // CAMERA_H
