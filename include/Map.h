#ifndef MAP_H
#define MAP_H

#include <GL/glut.h>
#include <SOIL/SOIL.h>


#include "Camera.h"

class Map
{
    public:
        Map();
        // Stockage des r�f�rences aux textures
        GLuint ListeTextures[20];
        GLuint Skybox[6];
        GLuint SolarSystemSkybox[6]; // Deuxi�me skybox
        void LoadTextures(void);
        void DrawGround(void);
        void DrawSkybox(Camera *cam);
        void DrawSolarSystemSkybox(Camera *cam);

    protected:
    private:

};

#endif // MAP_H
