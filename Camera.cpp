#include "include/Camera.h"
#include <cmath>


Camera::Camera()
{
    deltaForward = 0;
    deltaStrafe = 0;

    posx = 0.0f;
    posy = 1.5f;
    posz = 5.0f;

    dirx = 0.0f;
    diry = 0.0f;
    dirz = -1.0f;

    angleh = 0.0f;
    anglev = 0.0f;

    xOrigin = -1;
    yOrigin = -1;

    deltaAnglex = 0.0f;
    deltaAngley = 0.0f;



    locked = 1;
}

void Camera::updatePos()
{
    if (deltaForward || deltaStrafe)
    {
        if (locked)
        {
            posx += deltaForward * (dirx/cos(anglev + deltaAngley)) * MOVE_SPEED;
            posy += deltaForward * (diry/cos(anglev + deltaAngley)) * MOVE_SPEED;
            posz += deltaForward * (dirz/cos(anglev + deltaAngley)) * MOVE_SPEED;
            posx += deltaStrafe * (dirz/cos(anglev + deltaAngley)) * MOVE_SPEED;
            posz -= deltaStrafe * (dirx/cos(anglev + deltaAngley)) * MOVE_SPEED;

        }
        else
        {
            posx += deltaForward * dirx * MOVE_SPEED;
            posy += deltaForward * diry * MOVE_SPEED;
            posz += deltaForward * dirz * MOVE_SPEED;
            posx += deltaStrafe * dirz * MOVE_SPEED;
            posz -= deltaStrafe * dirx * MOVE_SPEED;
        }
    }
}

void Camera::orienterCam(int x, int y)
{
    if (xOrigin >= 0)
    {
        // Calculate the angle increments
        deltaAnglex = (x - xOrigin) * SENSIBILITY;
        deltaAngley = (y - yOrigin) * SENSIBILITY;

        // Create quaternions for the rotations
        Quaternion qx(cos(deltaAnglex / 2), 0.0f, sin(deltaAnglex / 2), 0.0f);
        Quaternion qy(cos(deltaAngley / 2), sin(deltaAngley / 2), 0.0f, 0.0f);

        // Update the camera's orientation
        orientation = qx * orientation * qy;

        // Normalize the quaternion to avoid drift
        orientation = orientation.normalize();

        // Update the direction vector using the quaternion
        Quaternion forward(0.0f, 0.0f, 0.0f, -1.0f); // Forward vector as a quaternion
        Quaternion rotatedForward = orientation * forward * orientation.conjugate();

        // Update the direction vector
        dirx = rotatedForward.x;
        diry = rotatedForward.y;
        dirz = rotatedForward.z;
    }
}



void Camera::releaseCam()
{
    angleh += deltaAnglex;
    anglev += deltaAngley;

    yOrigin = -1;
    xOrigin = -1;
}
void Camera::grabCam(int x, int y)
{
    xOrigin = x;
    yOrigin = y;
}
