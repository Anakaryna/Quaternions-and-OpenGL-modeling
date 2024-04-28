#ifndef CAMERA_H
#define CAMERA_H

#define MOVE_SPEED 0.1f
#define SENSIBILITY 0.005f

class Camera
{
    public:
        Camera();

        char locked;

        float posx;
        float posy;
        float posz;

        float dirx;
        float diry;
        float dirz;

        char deltaForward;
        char deltaStrafe;

        void updatePos(void);
        void releaseCam(void);
        void grabCam(int x, int y);
        void orienterCam(int x, int y);


    protected:
    private:


        float angleh;
        float anglev;

        int xOrigin;
        int yOrigin;
        float deltaAnglex;
        float deltaAngley;



};

#endif // CAMERA_H
