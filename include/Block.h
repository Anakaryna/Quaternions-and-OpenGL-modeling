#ifndef BLOCK_H
#define BLOCK_H

#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include "Quaternion.h"
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
#define SUN 8
#define MARS 9
#define JUPITER 10
#define MERCURY 11
#define NEPTUNE 17 // Pourquoi ca n'accepte pas 12
#define SATURN 13
#define URANUS 19
#define VENUS 15


#define TEXTURE_SCALE 1.0f

class Block
{
public:
    Block();
    Block(float x, float y, float z);
    void Draw();
    void DrawSphere(GLuint texture,float radius);  // Nouvelle méthode pour dessiner la sphère
    void SetTexture(int face, GLuint texture);
    void DrawSphere2(GLuint texture, float posX, float posY, float posZ, const Quaternion& rotation, const Quaternion& orbitRotation, float radius, float orbitRadius) ;
    void rotateSphere(GLUquadric* params, float radius, const Quaternion& rotation) ;
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

    //void rotateSphere(float d, const Quaternion quaternion);
};

#endif // BLOCK_H
