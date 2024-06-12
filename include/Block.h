#ifndef BLOCK_H
#define BLOCK_H

#include <GL/glut.h>
#include <SOIL/SOIL.h>

#define ANCHOR_FROM_ORIGIN 0
#define ANCHOR_FROM_CENTER 1

#define FRONT 0
#define BACK 1
#define LEFT 2
#define RIGHT 3
#define TOP 4
#define BOT 5
#define ALL 6
#define SPHERE 7

#define TEXTURE_SCALE 1.0f

class Block
{
public:
    Block();
    Block(float x, float y, float z);
    void Draw();
    void DrawSphere(GLuint texture,float radius);  // Nouvelle m�thode pour dessiner la sph�re
    void SetTexture(int face, GLuint texture);
    void DrawSphere2(GLuint texture, float posX, float posY, float posZ, float matrix4x4[16], float orbit_matrix4x4[16], float radius, float orbitRadius) ;
    float posx;
    float posy;
    float posz;


    float x;
    float y;
    float z;
protected:

private:
    GLuint textures[6];
    char anchor[3];
};

#endif // BLOCK_H
